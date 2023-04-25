export module cno:map;
import :blocktype;
import :random;
import casein;
import quack;

namespace cno {
static constexpr const auto map_width = 30;
static constexpr const auto map_height = 20;

struct block {
  const block_type *type;
  bool seen;
};

struct cell_size {
  unsigned w;
  unsigned h;
};

class map {
  unsigned m_level;
  cell_size m_cell;
  quack::grid_ilayout<map_width, map_height, block> m_blocks;

  [[nodiscard]] auto random_furniture() const noexcept {
    return cno::random(2) == 0 ? &star : &andsign;
  }

  void furnish_room(unsigned x1, unsigned y1, unsigned x2,
                    unsigned y2) noexcept {
    auto w = x2 - x1 + 1;
    auto h = y2 - y1 + 1;

    if (w == 3 && h == 3) {
      at(x1 + 1, y1 + 1) = random_furniture();
    } else if (w == 3) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        at(x1 + 1, y) = &vbar;
      }
      if ((h % 2) == 0) {
        at(x2 - 1, y2) = random_furniture();
      }
    } else if (h == 3) {
      for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
        at(x, y1 + 1) = &vbar;
      }
      if ((w % 2) == 0) {
        at(x2, y2 - 1) = random_furniture();
      }
    } else if ((w == 4) || (h == 4)) {
      for (auto y = y1 + 1; y <= y2 - 1; y++) {
        for (auto x = x1 + 1; x <= x2 - 1; x++) {
          at(x, y) = &tilda;
        }
      }
    } else if (w == 5) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        at(x1 + 1, y) = &vbar;
        at(x2 - 1, y) = &vbar;
      }
      if ((h % 2) == 0) {
        at(x2 - 2, y2) = random_furniture();
      }
    } else if (h == 5) {
      for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
        at(x, y1 + 1) = &vbar;
        at(x, y2 - 1) = &vbar;
      }
      if ((w % 2) == 0) {
        at(x2, y2 - 2) = random_furniture();
      }
    } else if ((w == 7) && (h == 7)) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
          at(x, y) = &vbar;
        }
      }
      at(x1 + 3, y1 + 3) = random_furniture();
    } else if ((w > 2) && (h > 2)) {
      for (auto y = y1; y <= y2; y++) {
        at(x1, y) = at(x2, y) = &comma;
      }
      for (auto x = x1; x <= x2; x++) {
        at(x, y1) = at(x, y2) = &comma;
      }
      if (((w == 6) && (h == 6)) || ((w == 9) && (h == 9))) {
        furnish_room(x1 + 1, y1 + 1, x2 - 1, y2 - 1);
      }
    }
  }

  unsigned subdivide_wide(unsigned x1, unsigned y1, unsigned x2,
                          unsigned y2) noexcept {
    auto w = x2 - x1 + 1;
    auto h = y2 - y1 + 1;

    if ((w < m_cell.w) || (h < m_cell.h) || (w == h)) {
      furnish_room(x1, y1, x2, y2);
      return 0;
    }

    auto x = x1 + 1;
    if (w != 3) {
      x += cno::random(w - 2);
    }

    for (auto y = y1; y <= y2; y++) {
      at(x, y) = &hash;
    }

    auto wall_1 = subdivide_high(x1, y1, x - 1, y2);
    auto wall_2 = subdivide_high(x + 1, y1, x2, y2);

    unsigned door_y;
    do {
      door_y = y1 + cno::random(h);
    } while ((door_y == wall_1) || (door_y == wall_2));

    at(x, door_y) = &dot;
    at(x - 1, door_y) = &dot;
    at(x + 1, door_y) = &dot;
    return x;
  }

  unsigned subdivide_high(unsigned x1, unsigned y1, unsigned x2,
                          unsigned y2) noexcept {
    auto w = x2 - x1 + 1;
    auto h = y2 - y1 + 1;

    if ((w < m_cell.w) || (h < m_cell.h) || (w == h)) {
      furnish_room(x1, y1, x2, y2);
      return 0;
    }

    auto y = y1 + 1;
    if (h != 3) {
      y += cno::random(h - 2);
    }

    for (auto x = x1; x <= x2; x++) {
      at(x, y) = &hash;
    }

    auto wall_1 = subdivide_wide(x1, y1, x2, y - 1);
    auto wall_2 = subdivide_wide(x1, y + 1, x2, y2);

    unsigned door_x;
    do {
      door_x = x1 + cno::random(w);
    } while ((door_x == wall_1) || (door_x == wall_2));

    at(door_x, y) = &dot;
    at(door_x, y - 1) = &dot;
    at(door_x, y + 1) = &dot;
    return y;
  }

  void fill_quack() noexcept {
    m_blocks.fill_colour([](const block &blk) {
      auto c = blk.type->character();
      auto r = static_cast<float>(c % 16) / 16.0f;
      auto g = static_cast<float>(c / 16) / 16.0f;
      auto b = blk.seen ? 1.0f : 0.0f;
      return quack::colour{r, g, b, 1};
    });
  }

public:
  constexpr map(quack::renderer *r) : m_blocks{r} {}

  [[nodiscard]] constexpr auto level() const noexcept { return m_level; }
  [[nodiscard]] constexpr const block_type *&at(unsigned x,
                                                unsigned y) noexcept {
    return m_blocks.at(x, y).type;
  }
  [[nodiscard]] constexpr const block_type *at(unsigned x,
                                               unsigned y) const noexcept {
    return m_blocks.at(x, y).type;
  }

  void set_level(unsigned l) noexcept {
    m_level = l;

    for (auto &b : m_blocks.data()) {
      b = {&dot};
    }

    for (auto x = 0; x < map_width; x++) {
      at(x, 0) = &hash;
      at(x, map_height - 1) = &hash;
    }
    for (int y = 0; y < map_height; y++) {
      at(0, y) = &hash;
      at(map_width - 1, y) = &hash;
    }

    static constexpr const auto max_cell_sizes = 5;
    static constexpr const cell_size cell_sizes[max_cell_sizes] = {
        {7, 7}, {7, 5}, {5, 5}, {5, 3}, {3, 3},
    };
    m_cell = cell_sizes[cno::random(max_cell_sizes)];

    subdivide_wide(1, 1, map_width - 2, map_height - 2);

    if ((m_level % 2) == 1) {
      at(map_width - 2, map_height - 2) = &gt;
    } else {
      at(1, map_height - 2) = &gt;
    }
    fill_quack();
  }

  void update_rogueview(unsigned px, unsigned py, unsigned radius) noexcept {
    for (auto x = 0; x < map_width; x++) {
      auto dx = px - x;
      for (auto y = 0; y < map_height; y++) {
        auto dy = py - y;
        if (dx * dx + dy * dy <= radius * radius) {
          m_blocks.at(x, y).seen = true;
        }
      }
    }
    fill_quack();
  }

  void process_event(const casein::event &e) noexcept {
    m_blocks.process_event(e);
  }
};
} // namespace cno
