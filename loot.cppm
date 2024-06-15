export module loot;
import dotz;
import hai;
import lootroll;
import map;
import qsu;
import spr;

export namespace loot {
struct loot {
  dotz::ivec2 coord{};
  spr::id spr{spr::nil};
};

hai::array<loot> list{map::height};

void init(int level) {
  for (auto y = 2; y < map::height - 2; y++) {
    auto x = map::pick_empty_space(y);
    if (x == -1) {
      list[y] = {};
      continue;
    }

    list[y] = {{x, y}, lootroll(level)};
  }
}

void draw() {
  for (auto &e : list) {
    qsu::blit(e.spr, e.coord.x, e.coord.y);
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
