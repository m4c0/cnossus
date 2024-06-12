#pragma leco add_resource "atlas.png"

export module play;
import dotz;
import enemies;
import loot;
import map;
import player;
import quack;
import voo;

namespace play {
export quack::donald::atlas_t *atlas(voo::device_and_queue *dq) {
  return new voo::sires_image("atlas.png", dq);
}

export void init(int level) {
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

export void move_by(int dx, int dy) {
  auto p = player::coord + dotz::ivec2{dx, dy};
  if (!map::can_walk(p.x, p.y))
    return;
  if (auto *e = enemies::at(p)) {
    enemies::hit(*e, 1);
    return;
  }

  player::coord = p;

  for (auto &e : enemies::list) {
    auto p = e.coord + enemies::next_move(e);
    if (!map::can_walk(p.x, p.y))
      continue;
    if (enemies::at(p))
      return;
    if (player::coord == p)
      continue;

    e.coord = p;
  }
}
} // namespace play
