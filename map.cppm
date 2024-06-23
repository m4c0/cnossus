export module map;
import dotz;
import rng;
import spr;
import qsu;

namespace map {
export constexpr const auto width = 30;
export constexpr const auto height = 20;

export spr::id data[height][width]{};
export bool visited[height][width]{};

export bool can_walk(unsigned x, unsigned y) {
  if (x >= width || y >= height)
    return false;

  auto id = data[y][x];
  if (id == spr::nil || id == spr::mosaic)
    return true;

  return false;
}

export void draw(dotz::ivec2 center, int radius) {
  auto [x, y] = center;
  for (auto yy = y - radius; yy <= y + radius; yy++) {
    if (yy < 0 || yy >= height)
      continue;
    for (auto xx = x - radius; xx <= x + radius; xx++) {
      if (xx < 0 || xx >= width)
        continue;
      visited[yy][xx] = true;
    }
  }

  for (auto y = 0; y < height; y++) {
    for (auto x = 0; x < width; x++) {
      if (!visited[y][x])
        continue;

      auto [dx, dy] = dotz::abs(center - dotz::ivec2{x, y});
      float m = (dx > radius || dy > radius) ? 0.6 : 1.0;
      qsu::guard::multiplier g{{1.0, 1.0, 1.0, m}};

      auto b = data[y][x];
      if (b != spr::wall && b != spr::pool)
        qsu::blit(spr::floor, x, y);
      qsu::blit(b, x, y);
    }
  }
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
  } else if (r.w == 3) {
    for (auto y = r.y0 + 1; y <= r.y1 - 1; y += 2) {
      data[y][r.x0 + 1] = spr::column;
    }
    if ((r.h % 2) == 0) {
      furniture(r.x0 + 1, r.y1);
    }
  } else if (r.h == 3) {
    for (auto x = r.x0 + 1; x <= r.x1 - 1; x += 2) {
      data[r.y0 + 1][x] = spr::column;
    }
    if ((r.w % 2) == 0) {
      furniture(r.x1, r.y0 + 1);
    }
  } else if (r.w == 4 || r.h == 4) {
    for (auto y = r.y0 + 1; y <= r.y1 - 1; y++) {
      for (auto x = r.x0 + 1; x <= r.x1 - 1; x++) {
        data[y][x] = spr::pool;
      }
    }
  } else if (r.w == 5) {
    for (auto y = r.y0 + 1; y <= r.y1 - 1; y += 2) {
      data[y][r.x0 + 1] = spr::column;
      data[y][r.x1 - 1] = spr::column;
    }
    if ((r.h % 2) == 0) {
      furniture(r.x1 - 2, r.y1);
    }
  } else if (r.h == 5) {
    for (auto x = r.x0 + 1; x <= r.x1 - 1; x += 2) {
      data[r.y0 + 1][x] = spr::column;
      data[r.y1 - 1][x] = spr::column;
    }
    if ((r.w % 2) == 0) {
      furniture(r.x1, r.y1 - 2);
    }
  } else if (r.w == 7 && r.h == 7) {
    for (auto y = r.y0 + 1; y <= r.y1 - 1; y += 2) {
      for (auto x = r.x0 + 1; x <= r.x1 - 1; x += 2) {
        data[y][x] = spr::column;
      }
    }
  } else if (r.w > 2 && r.h > 2) {
    for (auto y = r.y0; y <= r.y1; y++) {
      data[y][r.x0] = spr::mosaic;
      data[y][r.x1] = spr::mosaic;
    }
    for (auto x = r.x0; x <= r.x1; x++) {
      data[r.y0][x] = spr::mosaic;
      data[r.y1][x] = spr::mosaic;
    }
    if ((r.w == 6 && r.h == 6) || (r.w == 9 && r.h == 9)) {
      furnish({r.x0 + 1, r.y0 + 1, r.x1 - 1, r.y1 - 1, r.cell_w, r.cell_h});
    }
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

export void gen(int level) {
  for (auto &row : data) {
    for (auto &cell : row) {
      cell = {};
    }
  }
  for (auto &row : visited) {
    for (auto &cell : row) {
      cell = false;
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

  if ((level % 2) == 1) {
    data[height - 2][width - 2] = spr::exit;
  } else {
    data[height - 2][1] = spr::exit;
  }
}

export int pick_empty_space(unsigned y) {
  int x{};
  unsigned attempt{};
  do {
    x = rng::rand(width - 4) + 2;
  } while (data[y][x] != spr::nil && ++attempt < 100);
  return (attempt < 100) ? x : -1;
}
} // namespace map
