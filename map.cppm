export module map;
import rng;
import spr;
import quack;

namespace map {
export constexpr const auto width = 30;
export constexpr const auto height = 20;

export spr::id data[height][width]{};

export unsigned draw(quack::mapped_buffers &all) {
  unsigned count{};
  for (auto y = 0; y < height; y++) {
    for (auto x = 0; x < width; x++) {
      auto b = data[y][x];
      count += spr::blit(b, x, y, all);
    }
  }
  return count;
}

static void gen_rect(unsigned x0, unsigned y0, unsigned x1, unsigned y1) {
  for (auto x = x0; x <= x1; x++) {
    data[y0][x] = spr::wall;
    data[y1][x] = spr::wall;
  }
  for (auto y = y0; y <= y1; y++) {
    data[y][x0] = spr::wall;
    data[y][x1] = spr::wall;
  }
}
static void gen_w(unsigned x0, unsigned y0, unsigned w, unsigned h) {
  const auto x1 = x0 + w - 1;
  const auto y1 = y0 + h - 1;

  gen_rect(x0, y0, x1, y1);

  auto p = x0 + 1;
  if (w > 3)
    p += rng::rand(w) - 2;

  for (auto y = y0; y <= y1; y++) {
    data[y][p] = spr::wall;
  }
}

export void gen() {
  for (auto &row : data) {
    for (auto &cell : row) {
      cell = spr::floor;
    }
  }

  gen_w(0, 0, width, height);
}
} // namespace map
