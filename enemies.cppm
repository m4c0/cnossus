export module enemies;
import dotz;
import hai;
import map;
import mobroll;
import quack;
import rng;
import spr;

export namespace enemies {
struct enemy {
  dotz::ivec2 coord{};
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
    count += spr::blit(e.spr, e.coord.x, e.coord.y, all);
  }
  return count;
}

bool has(dotz::ivec2 p) {
  for (auto &e : list) {
    if (e.spr == spr::nil)
      continue;
    if (e.coord == p)
      return true;
  }
  return false;
}

dotz::ivec2 next_move(const enemy &e) {
  switch (e.spr) {
  case spr::nil:
    return {};
  default:
    switch (rng::rand(4)) {
    case 0:
      return {-1, 0};
    case 1:
      return {1, 0};
    case 2:
      return {0, -1};
    case 3:
      return {0, 1};
    default:
      return {};
    }
  }
}
} // namespace enemies
