export module loot;
import dotz;
import hai;
import lootroll;
import map;
import qsu;
import spr;

export namespace loot {
using loot = qsu::anim;

extern struct data {
  loot list[map::height * 2]{};
} d;

void init(int level) {
  d = {};

  for (auto y = 2; y < map::height - 2; y++) {
    auto x = map::pick_empty_space(y);
    if (x == -1) {
      d.list[y] = {};
      continue;
    }

    d.list[y] = loot{
        .coord = {x, y},
        .anim_coord = {x, y},
        .spr = lootroll(level),
    };
  }
}

void draw(dotz::vec2 center, int rad) {
  for (auto &e : d.list) {
    qsu::blit(e, center, rad, 0.6);
  }
}

loot *at(dotz::ivec2 p) {
  auto &l = d.list[p.y];
  if (l.coord.x != p.x)
    return nullptr;
  if (l.spr == spr::nil)
    return nullptr;

  return &l;
}
} // namespace loot

module :private;
loot::data loot::d;
