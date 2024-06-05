export module map;
import rng;
import spr;
import quack;

export namespace map {
constexpr const auto width = 30;
constexpr const auto height = 20;

spr::id data[height][width]{};

unsigned draw(quack::mapped_buffers &all) {
  unsigned count{};
  for (auto y = 0; y < height; y++) {
    for (auto x = 0; x < width; x++) {
      auto b = data[y][x];
      count += spr::blit(b, x, y, all);
    }
  }
  return count;
}

void gen() {
  for (auto y = 0U; y < height; y++) {
    for (auto x = 0U; x < width; x++) {
      data[y][x] = spr::floor;
    }
  }
  for (auto x = 0U; x < width; x++) {
    data[0][x] = spr::wall;
    data[height - 1][x] = spr::wall;
  }
  for (auto y = 0U; y < width; y++) {
    data[y][0] = spr::wall;
    data[y][width - 1] = spr::wall;
  }
}
} // namespace map
