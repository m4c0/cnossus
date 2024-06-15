#pragma leco add_resource "atlas.png"

export module play;
import dotz;
import enemies;
import inv;
import loot;
import lootroll;
import map;
import player;
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
  inv::init(level);
}

export auto draw(quack::mapped_buffers &all) {
  auto count = map::draw(all);
  count += loot::draw(all);
  count += enemies::draw(all);
  count += player::draw(all);
  count += inv::draw(all);
  return count;
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
  // TODO: poison damage

  if (player::life <= 0)
    return;

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
      auto enemy_def = enemies::life_of(e->spr);
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
      auto enemy_atk = enemies::life_of(e.spr);
      auto player_def = inv::defense() + player_base_def;
      auto poison = enemies::poison_of(e.spr);
      player::hit(enemy_atk - player_def, poison);
      continue;
    }

    e.coord = p;
  }
}
} // namespace play
