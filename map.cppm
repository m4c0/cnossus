export module map;
import rng;
import ecs;

namespace map {
// export constexpr const auto width = ecs::map_width;
// export constexpr const auto height = ecs::map_height;

export constexpr void add_background(ecs::ec *ec, unsigned x, unsigned y) {
  ecs::add_walkable_block(ec, '.', {x, y});
}
export constexpr void add_exit(ecs::ec *ec, unsigned x, unsigned y) {
  ecs::add_exit(ec, '<', {x, y});
}
export constexpr void add_wall(ecs::ec *ec, unsigned x, unsigned y) {
  ecs::add_rigid_block(ec, '&', {x, y});
}

export void create_maze(ecs::ec *ec, unsigned lvl, unsigned w, unsigned h);
export constexpr void create_room(ecs::ec *ec, unsigned w, unsigned h);
} // namespace map

module :private;

namespace map {
enum blocks : char {
  blank = ' ',
  dot = '.',
  hash = '&',
  comma = ',',
  tilda = '%',
  vbar = '#',
  star = '$',
  gt = '<',
  andsign = '(',
};

class maze {
  struct cell_size {
    unsigned w;
    unsigned h;
  };

  ecs::ec *m_ec;
  cell_size m_cell;
  rng::randomizer m_rng;

  [[nodiscard]] constexpr auto random(unsigned n) noexcept {
    return m_rng.random(n);
  }

  constexpr void add_rigid_block(unsigned x, unsigned y,
                                 auto blk) const noexcept {
    ecs::add_rigid_block(m_ec, blk, {x, y});
  }
  constexpr void add_walkable_block(unsigned x, unsigned y,
                                    auto blk) const noexcept {
    ecs::add_walkable_block(m_ec, blk, {x, y});
  }

  [[nodiscard]] constexpr auto random_furniture() noexcept {
    return random(2) == 0 ? star : andsign;
  }

  constexpr void remove_wall(unsigned x, unsigned y) {
    auto id = m_ec->blockers.get({x, y});
    if (!id)
      return;

    ecs::remove_wall(m_ec, id);
  }

  constexpr void furnish_room(unsigned x1, unsigned y1, unsigned x2,
                              unsigned y2) noexcept {
    auto w = x2 - x1 + 1;
    auto h = y2 - y1 + 1;

    if (w == 3 && h == 3) {
      add_rigid_block(x1 + 1, y1 + 1, random_furniture());
    } else if (w == 3) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        add_rigid_block(x1 + 1, y, vbar);
      }
      if ((h % 2) == 0) {
        add_rigid_block(x2 - 1, y2, random_furniture());
      }
    } else if (h == 3) {
      for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
        add_rigid_block(x, y1 + 1, vbar);
      }
      if ((w % 2) == 0) {
        add_rigid_block(x2, y2 - 1, random_furniture());
      }
    } else if ((w == 4) || (h == 4)) {
      for (auto y = y1 + 1; y <= y2 - 1; y++) {
        for (auto x = x1 + 1; x <= x2 - 1; x++) {
          add_rigid_block(x, y, tilda);
        }
      }
    } else if (w == 5) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        add_rigid_block(x1 + 1, y, vbar);
        add_rigid_block(x2 - 1, y, vbar);
      }
      if ((h % 2) == 0) {
        add_rigid_block(x2 - 2, y2, random_furniture());
      }
    } else if (h == 5) {
      for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
        add_rigid_block(x, y1 + 1, vbar);
        add_rigid_block(x, y2 - 1, vbar);
      }
      if ((w % 2) == 0) {
        add_rigid_block(x2, y2 - 2, random_furniture());
      }
    } else if ((w == 7) && (h == 7)) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
          add_rigid_block(x, y, vbar);
        }
      }
      add_rigid_block(x1 + 3, y1 + 3, random_furniture());
    } else if ((w > 2) && (h > 2)) {
      for (auto y = y1; y <= y2; y++) {
        add_walkable_block(x1, y, comma);
        add_walkable_block(x2, y, comma);
      }
      for (auto x = x1; x <= x2; x++) {
        add_walkable_block(x, y1, comma);
        add_walkable_block(x, y2, comma);
      }
      if (((w == 6) && (h == 6)) || ((w == 9) && (h == 9))) {
        furnish_room(x1 + 1, y1 + 1, x2 - 1, y2 - 1);
      }
    }
  }

  constexpr unsigned subdivide_wide(unsigned x1, unsigned y1, unsigned x2,
                                    unsigned y2) noexcept {
    auto w = x2 - x1 + 1;
    auto h = y2 - y1 + 1;

    if ((w < m_cell.w) || (h < m_cell.h) || (w == h)) {
      furnish_room(x1, y1, x2, y2);
      return 0;
    }

    auto x = x1 + 1;
    if (w != 3) {
      x += random(w - 2);
    }

    for (auto y = y1; y <= y2; y++) {
      add_rigid_block(x, y, hash);
    }

    auto wall_1 = subdivide_high(x1, y1, x - 1, y2);
    auto wall_2 = subdivide_high(x + 1, y1, x2, y2);

    unsigned door_y;
    do {
      door_y = y1 + random(h);
    } while ((door_y == wall_1) || (door_y == wall_2));

    remove_wall(x, door_y);
    remove_wall(x - 1, door_y);
    remove_wall(x + 1, door_y);
    return x;
  }

  constexpr unsigned subdivide_high(unsigned x1, unsigned y1, unsigned x2,
                                    unsigned y2) noexcept {
    auto w = x2 - x1 + 1;
    auto h = y2 - y1 + 1;

    if ((w < m_cell.w) || (h < m_cell.h) || (w == h)) {
      furnish_room(x1, y1, x2, y2);
      return 0;
    }

    auto y = y1 + 1;
    if (h != 3) {
      y += random(h - 2);
    }

    for (auto x = x1; x <= x2; x++) {
      add_rigid_block(x, y, hash);
    }

    auto wall_1 = subdivide_wide(x1, y1, x2, y - 1);
    auto wall_2 = subdivide_wide(x1, y + 1, x2, y2);

    unsigned door_x;
    do {
      door_x = x1 + random(w);
    } while ((door_x == wall_1) || (door_x == wall_2));

    remove_wall(door_x, y);
    remove_wall(door_x, y - 1);
    remove_wall(door_x, y + 1);
    return y;
  }

public:
  constexpr maze(ecs::ec *ec) : m_ec{ec} {}

  constexpr void build_level(unsigned lvl, unsigned width,
                             unsigned height) noexcept {
    create_room(m_ec, width, height);

    static constexpr const auto max_cell_sizes = 5;
    static constexpr const cell_size cell_sizes[max_cell_sizes] = {
        {7, 7}, {7, 5}, {5, 5}, {5, 3}, {3, 3},
    };
    m_cell = cell_sizes[random(max_cell_sizes)];

    subdivide_wide(1, 1, width - 2, height - 2);

    if ((lvl % 2) == 1) {
      add_exit(m_ec, width - 2, height - 2);
    } else {
      add_exit(m_ec, 1, height - 2);
    }
  }
};

void create_maze(ecs::ec *ec, unsigned lvl, unsigned w, unsigned h) {
  maze{ec}.build_level(lvl, w, h);
}

constexpr void create_room(ecs::ec *ec, unsigned w, unsigned h) {
  for (auto y = 0U; y < h; y++) {
    for (auto x = 0U; x < w; x++) {
      add_background(ec, x, y);
    }
  }

  for (auto x = 0; x < w; x++) {
    add_wall(ec, x, 0);
    add_wall(ec, x, h - 1);
  }
  for (int y = 0; y < h; y++) {
    add_wall(ec, 0, y);
    add_wall(ec, w - 1, y);
  }
}
} // namespace map

static_assert([] {
  ecs::ec ec{};
  map::maze{&ec}.build_level(1, 30, 20);
  return true;
}());
