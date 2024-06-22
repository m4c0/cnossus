export module play;
import dotz;
import enemies;
import inv;
import light;
import loot;
import lootroll;
import map;
import player;
import qsu;
import quack;
import spr;

namespace play {
int level{};

export void init(int lvl) {
  level = lvl;
  map::gen(level);
  player::init(level);
  enemies::init(level);
  loot::init(level);
  light::init(level);
  inv::init(level);

  quack::donald::push_constants({
      .grid_pos = {},
      .grid_size = {9, 9},
  });
}

export void draw() {
  auto radius = light::charge > 0 ? 2 : 1;
  {
    qsu::guard::position ppo{-(player::d.coord + 0.5f)};
    map::draw(player::d.coord, radius);
    loot::draw(player::d.coord, radius);
    enemies::draw(player::d.coord, radius);
    player::draw();
  }

  player::draw_ui();
  light::draw_ui();
  inv::draw_ui();
}

export void eat(int i) {
  auto roll = food_of(inv::eat_food(i));
  if (roll == 0)
    return;

  player::restore(roll);
}

export void light(int i) {
  auto roll = light_of(inv::burn_light(i));
  if (roll == 0)
    return;

  light::restore(roll);
}

static void take_loot(loot::loot *l) {
  switch (l->spr) {
  case spr::jar:
  case spr::coffer:
    l->spr = lootroll(level, l->spr);
    break;
  default:
    player::d.coord = l->coord;
    if (inv::take(l->spr))
      l->spr = spr::nil;
    break;
  }
}

export void move_by(int dx, int dy) {
  if (player::d.life <= 0)
    return;

  auto p = player::d.coord + dotz::ivec2{dx, dy};
  if (map::data[p.y][p.x] == spr::exit) {
    init(level + 1);
    return;
  }
  if (!map::can_walk(p.x, p.y))
    return;

  player::poison_tick();
  light::tick();

  if (auto *e = enemies::at(p)) {
    if (e->life > 0) {
      auto player_atk = inv::attack() + player::d.attack;
      auto enemy_def = life_of(e->spr);
      enemies::hit(*e, player_atk - enemy_def);
    } else if (e->spr != spr::nil) {
      player::d.coord = p;
      if (inv::take(e->spr))
        e->spr = spr::nil;
    }
  } else if (auto *l = loot::at(p)) {
    take_loot(l);
  } else {
    player::d.coord = p;
  }

  for (auto &e : enemies::list) {
    if (e.spr == spr::nil || e.life == 0)
      continue;

    auto p = e.coord + enemies::next_move(e);
    if (!map::can_walk(p.x, p.y))
      continue;
    if (enemies::at(p))
      continue;
    if (player::d.coord == p) {
      auto enemy_atk = life_of(e.spr);
      auto player_def = inv::defense() + player::d.defense;
      auto poison = poison_of(e.spr);
      player::hit(enemy_atk - player_def, poison);
      continue;
    }

    e.coord = p;
  }
}
} // namespace play
