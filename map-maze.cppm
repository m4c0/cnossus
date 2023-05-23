export module map:maze;
import :block;
import ecs;
import rng;

namespace map {
export constexpr const auto width = ecs::map_width;
export constexpr const auto height = ecs::map_height;

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
    ecs::add_rigid_block(m_ec, blk.id, {x, y});
  }
  constexpr void add_walkable_block(unsigned x, unsigned y,
                                    auto blk) const noexcept {
    ecs::add_walkable_block(m_ec, blk.id, {x, y});
  }

  [[nodiscard]] constexpr auto random_furniture() noexcept {
    return random(2) == 0 ? star : andsign;
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
        add_walkable_block(x, y2, comma);
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

    ecs::wipeout_entity(m_ec, m_ec->blockers.get({x, door_y}));
    ecs::wipeout_entity(m_ec, m_ec->blockers.get({x - 1, door_y}));
    ecs::wipeout_entity(m_ec, m_ec->blockers.get({x + 1, door_y}));
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

    ecs::wipeout_entity(m_ec, m_ec->blockers.get({door_x, y}));
    ecs::wipeout_entity(m_ec, m_ec->blockers.get({door_x, y - 1}));
    ecs::wipeout_entity(m_ec, m_ec->blockers.get({door_x, y + 1}));
    return y;
  }

public:
  constexpr maze(ecs::ec *ec) : m_ec{ec} {}

  constexpr void build_level(unsigned lvl) noexcept {
    for (auto y = 0U; y < ecs::map_height; y++) {
      for (auto x = 0U; x < ecs::map_width; x++) {
        add_walkable_block(x, y, dot);
      }
    }

    for (auto x = 0; x < width; x++) {
      add_rigid_block(x, 0, hash);
      add_rigid_block(x, height - 1, hash);
    }
    for (int y = 0; y < height; y++) {
      add_rigid_block(0, y, hash);
      add_rigid_block(width - 1, y, hash);
    }

    static constexpr const auto max_cell_sizes = 5;
    static constexpr const cell_size cell_sizes[max_cell_sizes] = {
        {7, 7}, {7, 5}, {5, 5}, {5, 3}, {3, 3},
    };
    m_cell = cell_sizes[random(max_cell_sizes)];

    subdivide_wide(1, 1, width - 2, height - 2);

    if ((lvl % 2) == 1) {
      ecs::add_exit(m_ec, gt.id, {width - 2, height - 2});
    } else {
      ecs::add_exit(m_ec, gt.id, {1, height - 2});
    }
  }
};
} // namespace map

namespace {
static_assert([] {
  ecs::ec ec{};
  map::maze{&ec}.build_level(1);
  return true;
}());
}
