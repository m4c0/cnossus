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
import save;
import spr;
import timeline;

namespace play {
bool g_animating{};
unsigned level{};

void init(unsigned lvl) {
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
export void init() {
  auto d = save::read();
  init(d.level + 1);
}
void next_level() {
  save::write({.level = level});
  init(level + 1);
}

export void draw() {
  g_animating = tim::tick();

  auto pac = player::anim_coord();
  auto radius = light::charge > 0 ? 2 : 1;
  {
    qsu::guard::position ppo{-(pac + 0.5f)};
    map::draw(pac, radius);
    loot::draw(pac, radius);
    enemies::draw(pac, radius);
    player::draw();
  }

  player::draw_ui();
  light::draw_ui();
  inv::draw_ui();
}

export bool is_animating() { return g_animating; }

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
    player::move(l->coord);
    if (inv::take(l->spr))
      l->spr = spr::nil;
    break;
  }
}

export void move_by(int dx, int dy) {
  tim::reset();

  if (player::is_dead())
    return;

  auto p = player::coord() + dotz::ivec2{dx, dy};
  if (map::data[p.y][p.x].spr == spr::exit) {
    next_level();
    return;
  }
  if (!map::can_walk(p.x, p.y))
    return;

  player::poison_tick();
  light::tick();

  if (auto *e = enemies::at(p)) {
    if (e->life > 0) {
      auto player_atk = inv::attack() + player::attack();
      auto enemy_def = life_of(e->spr);
      enemies::hit(*e, player_atk - enemy_def);
    } else if (e->spr != spr::nil) {
      player::move(p);
      if (inv::take(e->spr))
        e->spr = spr::nil;
    }
  } else if (auto *l = loot::at(p)) {
    take_loot(l);
  } else {
    player::move(p);
  }

  for (auto &e : enemies::list) {
    if (e.spr == spr::nil || e.life == 0)
      continue;

    auto p =
        e.coord + enemies::next_move(e, player::coord(), light::charge > 0);
    if (!map::can_walk(p.x, p.y))
      continue;
    if (enemies::at(p))
      continue;
    if (player::coord() == p) {
      auto enemy_atk = life_of(e.spr);
      auto player_def = inv::defense() + player::defense();
      auto poison = poison_of(e.spr);
      player::hit(enemy_atk - player_def, poison);
      continue;
    }

    enemies::move(e, p);
  }
}
} // namespace play
