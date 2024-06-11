export module enemies;
import dotz;
import hai;
import map;
import mobroll;
import quack;
import spr;

export namespace enemies {
struct enemy {
  dotz::ivec2 pos{};
  spr::id spr{spr::nil};
};

hai::array<enemy> list{map::height};

void init(int level) {
  for (auto y = 2; y < map::height - 2; y++) {
    auto x = map::pick_empty_space(y);
    if (x == -1) {
      list[y] = {};
      continue;
    }

    list[y] = {{x, y}, mobroll(level)};
  }
}

auto draw(quack::mapped_buffers &all) {
  unsigned count{};
  for (auto &e : list) {
    count += spr::blit(e.spr, e.pos.x, e.pos.y, all);
  }
  return count;
}
} // namespace enemies
