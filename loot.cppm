export module loot;
import dotz;
import hai;
import lootroll;
import map;
import quack;
import spr;

export namespace loot {
struct loot {
  dotz::ivec2 pos{};
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

auto draw(quack::mapped_buffers &all) {
  unsigned count{};
  for (auto &e : list) {
    count += spr::blit(e.spr, e.pos.x, e.pos.y, all);
  }
  return count;
}
} // namespace loot
