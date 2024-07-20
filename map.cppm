export module map;
import dotz;
import rng;
import spr;
import qsu;
import yoyo;

namespace map {
export constexpr const auto width = 30;
export constexpr const auto height = 20;

export struct {
  qsu::anim data[height][width];
} real_data;
auto &data = real_data.data;

export auto at(unsigned x, unsigned y) { return data[y][x].spr; }
export bool can_walk(unsigned x, unsigned y) {
  if (x >= width || y >= height)
    return false;

  auto id = data[y][x].spr;
  if (id == spr::nil || id == spr::mosaic)
    return true;

  return false;
}

export void draw(dotz::vec2 center, int radius, bool play_area_limit = true) {
  for (auto y = 0; y < height; y++) {
    for (auto x = 0; x < width; x++) {
      auto b = data[y][x];
      if (b.spr != spr::wall && b.spr != spr::pool) {
        b.spr = spr::floor;
        qsu::blit(b, center, radius, 0.6);
      }
      qsu::blit(data[y][x], center, radius, 0.6);
    }
  }
}

void furniture(unsigned x, unsigned y) {
  data[y][x].spr = rng::rand(2) ? spr::basin : spr::statue;
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
      data[y][r.x0 + 1].spr = spr::column;
    }
    if ((r.h % 2) == 0) {
      furniture(r.x0 + 1, r.y1);
    }
  } else if (r.h == 3) {
    for (auto x = r.x0 + 1; x <= r.x1 - 1; x += 2) {
      data[r.y0 + 1][x].spr = spr::column;
    }
    if ((r.w % 2) == 0) {
      furniture(r.x1, r.y0 + 1);
    }
  } else if (r.w == 4 || r.h == 4) {
    for (auto y = r.y0 + 1; y <= r.y1 - 1; y++) {
      for (auto x = r.x0 + 1; x <= r.x1 - 1; x++) {
        data[y][x].spr = spr::pool;
      }
    }
  } else if (r.w == 5) {
    for (auto y = r.y0 + 1; y <= r.y1 - 1; y += 2) {
      data[y][r.x0 + 1].spr = spr::column;
      data[y][r.x1 - 1].spr = spr::column;
    }
    if ((r.h % 2) == 0) {
      furniture(r.x1 - 2, r.y1);
    }
  } else if (r.h == 5) {
    for (auto x = r.x0 + 1; x <= r.x1 - 1; x += 2) {
      data[r.y0 + 1][x].spr = spr::column;
      data[r.y1 - 1][x].spr = spr::column;
    }
    if ((r.w % 2) == 0) {
      furniture(r.x1, r.y1 - 2);
    }
  } else if (r.w == 7 && r.h == 7) {
    for (auto y = r.y0 + 1; y <= r.y1 - 1; y += 2) {
      for (auto x = r.x0 + 1; x <= r.x1 - 1; x += 2) {
        data[y][x].spr = spr::column;
      }
    }
  } else if (r.w > 2 && r.h > 2) {
    for (auto y = r.y0; y <= r.y1; y++) {
      data[y][r.x0].spr = spr::mosaic;
      data[y][r.x1].spr = spr::mosaic;
    }
    for (auto x = r.x0; x <= r.x1; x++) {
      data[r.y0][x].spr = spr::mosaic;
      data[r.y1][x].spr = spr::mosaic;
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
    data[p][x].spr = spr::wall;
  }

  auto r1 = split_w({r.x0, r.y0, r.x1, p - 1, r.cell_w, r.cell_h});
  auto r2 = split_w({r.x0, p + 1, r.x1, r.y1, r.cell_w, r.cell_h});

  unsigned door{};
  do {
    door = r.x0 + rng::rand(r.w);
  } while (door == r1 || door == r2);
  data[p - 1][door].spr = {};
  data[p][door].spr = {};
  data[p + 1][door].spr = {};

  return p;
}
static unsigned split_w(const room &r) {
  if (furnish(r))
    return 0;

  auto p = r.x0 + 1;
  if (r.w > 3)
    p += rng::rand(r.w - 2);

  for (auto y = r.y0; y <= r.y1; y++) {
    data[y][p].spr = spr::wall;
  }

  auto r1 = split_h({r.x0, r.y0, p - 1, r.y1, r.cell_w, r.cell_h});
  auto r2 = split_h({p + 1, r.y0, r.x1, r.y1, r.cell_w, r.cell_h});

  unsigned door{};
  do {
    door = r.y0 + rng::rand(r.h);
  } while (door == r1 || door == r2);
  data[door][p - 1].spr = {};
  data[door][p].spr = {};
  data[door][p + 1].spr = {};

  return p;
}

export void gen(int level) {
  for (auto y = 0; y < height; y++) {
    for (auto x = 0; x < width; x++) {
      data[y][x] = {.coord = {x, y}, .anim_coord = {x, y}};
    }
  }

  for (auto x = 0; x < width; x++) {
    data[0][x].spr = spr::wall;
    data[height - 1][x].spr = spr::wall;
  }
  for (auto y = 0; y < height; y++) {
    data[y][0].spr = spr::wall;
    data[y][width - 1].spr = spr::wall;
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
    data[height - 2][width - 2].spr = spr::exit;
  } else {
    data[height - 2][1].spr = spr::exit;
  }
}

export int pick_empty_space(unsigned y) {
  int x{};
  unsigned attempt{};
  do {
    x = rng::rand(width - 4) + 2;
  } while (data[y][x].spr != spr::nil && ++attempt < 100);
  return (attempt < 100) ? x : -1;
}
} // namespace map
