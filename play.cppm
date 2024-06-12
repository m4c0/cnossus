#pragma leco add_resource "atlas.png"

export module play;
import dotz;
import enemies;
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
}

export auto draw(quack::mapped_buffers &all) {
  auto count = map::draw(all);
  count += loot::draw(all);
  count += enemies::draw(all);
  count += player::draw(all);
  return count;
}

static void take_loot(loot::loot *l) {
  switch (l->spr) {
  case spr::jar:
  case spr::coffer:
    l->spr = lootroll(level, l->spr);
    break;
  default:
    player::coord = l->pos;
    break;
  }
}

export void move_by(int dx, int dy) {
  if (player::life == 0)
    return;

  auto p = player::coord + dotz::ivec2{dx, dy};
  if (map::data[p.y][p.x] == spr::exit) {
    init(level + 1);
    return;
  }
  if (!map::can_walk(p.x, p.y))
    return;
  if (auto *e = enemies::at(p)) {
    enemies::hit(*e, 1);
  } else if (auto *l = loot::at(p)) {
    take_loot(l);
  } else {
    player::coord = p;
  }

  for (auto &e : enemies::list) {
    auto p = e.coord + enemies::next_move(e);
    if (!map::can_walk(p.x, p.y))
      continue;
    if (enemies::at(p))
      continue;
    if (player::coord == p) {
      // TODO: attack roll
      // TODO: game over
      player::hit(1);
      continue;
    }

    e.coord = p;
  }
}
} // namespace play
