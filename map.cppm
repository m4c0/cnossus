export module map;
import rng;
import silog;
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

static unsigned split_w(unsigned x0, unsigned y0, unsigned x1, unsigned y1);
static unsigned split_h(unsigned x0, unsigned y0, unsigned x1, unsigned y1) {
  const auto w = x1 - x0 + 1;
  const auto h = y1 - y0 + 1;

  if (h <= 5)
    return 0;

  auto p = y0 + 1;
  if (h > 3)
    p += rng::rand(h - 2);

  for (auto x = x0; x <= x1; x++) {
    data[p][x] = spr::wall;
  }

  auto r1 = split_w(x0, y0, x1, p - 1);
  auto r2 = split_w(x0, p + 1, x1, y1);

  unsigned door{};
  do {
    door = x0 + rng::rand(w);
  } while (door == r1 || door == r2);
  data[p - 1][door] = spr::floor;
  data[p][door] = spr::floor;
  data[p + 1][door] = spr::floor;

  return p;
}
static unsigned split_w(unsigned x0, unsigned y0, unsigned x1, unsigned y1) {
  const auto w = x1 - x0 + 1;
  const auto h = y1 - y0 + 1;

  if (w <= 5)
    return 0;

  auto p = x0 + 1;
  if (w > 3)
    p += rng::rand(w - 2);

  for (auto y = y0; y <= y1; y++) {
    data[y][p] = spr::wall;
  }

  auto r1 = split_h(x0, y0, p - 1, y1);
  auto r2 = split_h(p + 1, y0, x1, y1);

  unsigned door{};
  do {
    door = y0 + rng::rand(h);
  } while (door == r1 || door == r2);
  data[door][p - 1] = spr::floor;
  data[door][p] = spr::floor;
  data[door][p + 1] = spr::floor;

  return p;
}

export void gen() {
  for (auto &row : data) {
    for (auto &cell : row) {
      cell = spr::floor;
    }
  }
  for (auto x = 0; x < width; x++) {
    data[0][x] = spr::wall;
    data[height - 1][x] = spr::wall;
  }
  for (auto y = 0; y < height; y++) {
    data[y][0] = spr::wall;
    data[y][width - 1] = spr::wall;
  }

  split_w(1, 1, width - 2, height - 2);
}
} // namespace map
