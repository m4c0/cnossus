export module loot;
import dotz;
import hai;
import lootroll;
import map;
import qsu;
import spr;

export namespace loot {
using loot = qsu::spr;
hai::array<loot> list{map::height};

void init(int level) {
  for (auto y = 2; y < map::height - 2; y++) {
    auto x = map::pick_empty_space(y);
    if (x == -1) {
      list[y] = {};
      continue;
    }

    list[y] = loot{
        .coord = {x, y},
        .anim_coord = {x, y},
        .spr = lootroll(level),
    };
  }
}

void draw(dotz::vec2 center, int rad) {
  for (auto &e : list) {
    qsu::blit(e, center, rad, 0.6);
  }
}

loot *at(dotz::ivec2 p) {
  auto &l = list[p.y];
  if (l.coord.x != p.x)
    return nullptr;
  if (l.spr == spr::nil)
    return nullptr;

  return &l;
}
} // namespace loot
