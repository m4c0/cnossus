module;
extern "C" int rand();

export module cno:map;
import :blocktype;

namespace cno {
static constexpr const auto map_width = 30;
static constexpr const auto map_height = 20;

struct block {
  const block_type *type;
  bool seen;
};

class map {
  unsigned m_level;
  unsigned m_cell_w;
  unsigned m_cell_h;
  block m_blocks[map_width * map_height];

  constexpr void fill(const block_type *t) noexcept {
    for (auto &b : m_blocks) {
      b.type = t;
    }
  }

  [[nodiscard]] auto random_furniture() const noexcept {
    return rand() % 2 == 0 ? &star : &andsign;
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
        at(x, y1 + 1) = &tilda;
      }
      if ((h % 2) == 0) {
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
        furnish_room(x1 + 1, y1 + 2, x2 - 1, y2 - 1);
      }
    }
  }

  [[nodiscard]] unsigned subdivide_wide(unsigned x1, unsigned y1, unsigned x2,
                                        unsigned y2) noexcept {
    auto w = x2 - x1 + 1;
    auto h = y2 - y1 + 1;

    if ((w < m_cell_w) || (h < m_cell_h) || (w == h)) {
      furnish_room(x1, y1, x2, y2);
      return 0;
    }

    auto x = x1 + 1;
    if (w != 3) {
      x += rand() % (w - 2);
    }

    for (auto y = y1; y <= y2; y++) {
      at(x, y) = &hash;
    }

    auto wall_1 = subdivide_high(x1, y1, x - 1, y2);
    auto wall_2 = subdivide_high(x + 1, y1, x2, y2);

    unsigned door_y;
    do {
      door_y = y1 + rand() % h;
    } while ((door_y == wall_1) || (door_y == wall_2));

    at(x, door_y) = &dot;
    at(x - 1, door_y) = &dot;
    at(x + 1, door_y) = &dot;
    return x;
  }

  [[nodiscard]] unsigned subdivide_high(unsigned x1, unsigned y1, unsigned x2,
                                        unsigned y2) noexcept {
    auto w = x2 - x1 + 1;
    auto h = y2 - y1 + 1;

    if ((w < m_cell_w) || (h < m_cell_h) || (w == h)) {
      furnish_room(x1, y1, x2, y2);
      return 0;
    }

    auto y = y1 + 1;
    if (h != 3) {
      y += rand() % (h - 2);
    }

    for (auto x = x1; x <= x2; x++) {
      at(x, y) = &hash;
    }

    auto wall_1 = subdivide_high(x1, y1, x2, y - 1);
    auto wall_2 = subdivide_high(x1, y + 1, x2, y2);

    unsigned door_x;
    do {
      door_x = x1 + rand() % h;
    } while ((door_x == wall_1) || (door_x == wall_2));

    at(door_x, y) = &dot;
    at(door_x, y - 1) = &dot;
    at(door_x, y + 1) = &dot;
    return y;
  }

public:
  [[nodiscard]] constexpr const block_type *&at(unsigned x,
                                                unsigned y) noexcept {
    return m_blocks[y * map_width + x].type;
  }
  [[nodiscard]] constexpr const block_type *at(unsigned x,
                                               unsigned y) const noexcept {
    return m_blocks[y * map_width + x].type;
  }
};
} // namespace cno
