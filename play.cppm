#pragma leco add_resource "atlas.png"

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
import voo;

namespace play {
export quack::donald::atlas_t *atlas(voo::device_and_queue *dq) {
  return new voo::sires_image("atlas.png", dq);
}

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
  {
    qsu::guard::position ppo{-(player::coord + 0.5f)};
    map::draw();
    loot::draw();
    enemies::draw();
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
    player::coord = l->coord;
    if (inv::take(l->spr))
      l->spr = spr::nil;
    break;
  }
}

export void move_by(int dx, int dy) {
  // TODO: light diminish

  if (player::life <= 0)
    return;

  player::poison_tick();
  light::tick();

  auto p = player::coord + dotz::ivec2{dx, dy};
  if (map::data[p.y][p.x] == spr::exit) {
    init(level + 1);
    return;
  }
  if (!map::can_walk(p.x, p.y))
    return;
  if (auto *e = enemies::at(p)) {
    if (e->life > 0) {
      constexpr const auto player_base_attack = 10;
      auto player_atk = inv::attack() + player_base_attack;
      auto enemy_def = life_of(e->spr);
      enemies::hit(*e, player_atk - enemy_def);
    } else if (e->spr != spr::nil) {
      player::coord = p;
      if (inv::take(e->spr))
        e->spr = spr::nil;
    }
  } else if (auto *l = loot::at(p)) {
    take_loot(l);
  } else {
    player::coord = p;
  }

  for (auto &e : enemies::list) {
    if (e.spr == spr::nil || e.life == 0)
      continue;

    auto p = e.coord + enemies::next_move(e);
    if (!map::can_walk(p.x, p.y))
      continue;
    if (enemies::at(p))
      continue;
    if (player::coord == p) {
      constexpr const auto player_base_def = 10;
      auto enemy_atk = life_of(e.spr);
      auto player_def = inv::defense() + player_base_def;
      auto poison = poison_of(e.spr);
      player::hit(enemy_atk - player_def, poison);
      continue;
    }

    e.coord = p;
  }
}
} // namespace play
