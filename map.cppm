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

void furniture(unsigned x, unsigned y) {
  data[y][x] = rng::rand(2) ? spr::basin : spr::statue;
}

struct room {
  unsigned x0;
  unsigned y0;
  unsigned x1;
  unsigned y1;
  unsigned cell_w;
  unsigned cell_h;
  unsigned w = x1 - x0 + 1;
  unsigned h = y1 - y0 + 1;
};
static constexpr bool match_level_cell(const room &r) {
  return r.w < r.cell_w || r.h < r.cell_h;
}
static bool furnish(const room &r) {
  if (!match_level_cell(r))
    return false;

  if (r.w == 3 && r.h == 3) {
    furniture(r.x0 + 1, r.y0 + 1);
  }
  return true;
}

static unsigned split_w(const room &r);
static unsigned split_h(const room &r) {
  if (furnish(r))
    return 0;

  auto p = r.y0 + 1;
  if (r.h > 3)
    p += rng::rand(r.h - 2);

  for (auto x = r.x0; x <= r.x1; x++) {
    data[p][x] = spr::wall;
  }

  auto r1 = split_w({r.x0, r.y0, r.x1, p - 1, r.cell_w, r.cell_h});
  auto r2 = split_w({r.x0, p + 1, r.x1, r.y1, r.cell_w, r.cell_h});

  unsigned door{};
  do {
    door = r.x0 + rng::rand(r.w);
  } while (door == r1 || door == r2);
  data[p - 1][door] = {};
  data[p][door] = {};
  data[p + 1][door] = {};

  return p;
}
static unsigned split_w(const room &r) {
  if (furnish(r))
    return 0;

  auto p = r.x0 + 1;
  if (r.w > 3)
    p += rng::rand(r.w - 2);

  for (auto y = r.y0; y <= r.y1; y++) {
    data[y][p] = spr::wall;
  }

  auto r1 = split_h({r.x0, r.y0, p - 1, r.y1, r.cell_w, r.cell_h});
  auto r2 = split_h({p + 1, r.y0, r.x1, r.y1, r.cell_w, r.cell_h});

  unsigned door{};
  do {
    door = r.y0 + rng::rand(r.h);
  } while (door == r1 || door == r2);
  data[door][p - 1] = {};
  data[door][p] = {};
  data[door][p + 1] = {};

  return p;
}

export void gen() {
  for (auto &row : data) {
    for (auto &cell : row) {
      cell = {};
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

  // "cell" gives an "accent" to generated levels - smaller cells makes more
  // thin corridors, whilst larger yields more open rooms
  static constexpr const auto max_cell_sizes = 5;
  static constexpr const struct {
    unsigned x;
    unsigned y;
  } cell_sizes[max_cell_sizes] = {
      {7, 7}, {7, 5}, {5, 5}, {5, 3}, {3, 3},
  };
  auto [cw, ch] = cell_sizes[rng::rand(max_cell_sizes)];
  split_w({1, 1, width - 2, height - 2, cw, ch});
}
} // namespace map
