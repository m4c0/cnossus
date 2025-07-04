export module loot;
import anim;
import dotz;
import hai;
import lootroll;
import map;
import spr;

export namespace loot {
using loot = anim::t;

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

void draw() {
  for (auto &e : d.list) {
    anim::blit(e, false);
  }
}

loot *at(dotz::ivec2 p) {
  for (auto &l : d.list) {
    if (l.spr == spr::nil)
      continue;
    if (l.coord != p)
      continue;

    return &l;
  }

  return nullptr;
}

void add(dotz::ivec2 p, spr::id spr) {
  for (auto &l : d.list) {
    if (l.spr != spr::nil)
      continue;

    l = loot{
        .coord = p,
        .anim_coord = p,
        .spr = spr,
    };
    return;
  }
}

} // namespace loot

module :private;
loot::data loot::d;
