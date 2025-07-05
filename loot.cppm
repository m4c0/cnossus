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

  map::pick_empty_spaces([level, l=d.list](auto p) mutable {
    *l++ = loot {
      .coord = p,
      .spr = lootroll(level),
    };
  });
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
