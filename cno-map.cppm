export module cno:map;
import :blocktype;
import :globals;
import :random;
import casein;
import quack;

namespace cno {
enum block_vibility { bv_none, bv_fog, bv_visible };
struct block {
  sprite<block_type> type;
  block_vibility vis;
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
    return sprite{cno::random(2) == 0 ? &star : &andsign};
  }

  void furnish_room(unsigned x1, unsigned y1, unsigned x2,
                    unsigned y2) noexcept {
    auto w = x2 - x1 + 1;
    auto h = y2 - y1 + 1;

    if (w == 3 && h == 3) {
      at(x1 + 1, y1 + 1) = random_furniture();
    } else if (w == 3) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        at(x1 + 1, y) = sprite{&vbar};
      }
      if ((h % 2) == 0) {
        at(x2 - 1, y2) = random_furniture();
      }
    } else if (h == 3) {
      for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
        at(x, y1 + 1) = sprite{&vbar};
      }
      if ((w % 2) == 0) {
        at(x2, y2 - 1) = random_furniture();
      }
    } else if ((w == 4) || (h == 4)) {
      for (auto y = y1 + 1; y <= y2 - 1; y++) {
        for (auto x = x1 + 1; x <= x2 - 1; x++) {
          at(x, y) = sprite{&tilda};
        }
      }
    } else if (w == 5) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        at(x1 + 1, y) = sprite{&vbar};
        at(x2 - 1, y) = sprite{&vbar};
      }
      if ((h % 2) == 0) {
        at(x2 - 2, y2) = random_furniture();
      }
    } else if (h == 5) {
      for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
        at(x, y1 + 1) = sprite{&vbar};
        at(x, y2 - 1) = sprite{&vbar};
      }
      if ((w % 2) == 0) {
        at(x2, y2 - 2) = random_furniture();
      }
    } else if ((w == 7) && (h == 7)) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
          at(x, y) = sprite{&vbar};
        }
      }
      at(x1 + 3, y1 + 3) = random_furniture();
    } else if ((w > 2) && (h > 2)) {
      for (auto y = y1; y <= y2; y++) {
        at(x1, y) = at(x2, y) = sprite{&comma};
      }
      for (auto x = x1; x <= x2; x++) {
        at(x, y1) = at(x, y2) = sprite{&comma};
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
      at(x, y) = sprite{&hash};
    }

    auto wall_1 = subdivide_high(x1, y1, x - 1, y2);
    auto wall_2 = subdivide_high(x + 1, y1, x2, y2);

    unsigned door_y;
    do {
      door_y = y1 + cno::random(h);
    } while ((door_y == wall_1) || (door_y == wall_2));

    at(x, door_y) = sprite{&dot};
    at(x - 1, door_y) = sprite{&dot};
    at(x + 1, door_y) = sprite{&dot};
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
      at(x, y) = sprite{&hash};
    }

    auto wall_1 = subdivide_wide(x1, y1, x2, y - 1);
    auto wall_2 = subdivide_wide(x1, y + 1, x2, y2);

    unsigned door_x;
    do {
      door_x = x1 + cno::random(w);
    } while ((door_x == wall_1) || (door_x == wall_2));

    at(door_x, y) = sprite{&dot};
    at(door_x, y - 1) = sprite{&dot};
    at(door_x, y + 1) = sprite{&dot};
    return y;
  }

  void update_rogueview(map_coord pc, unsigned radius) noexcept {
    for (auto x = 0; x < map_width; x++) {
      auto dx = pc.x - x;
      for (auto y = 0; y < map_height; y++) {
        auto dy = pc.y - y;
        auto &vis = m_blocks.at(x, y).vis;
        if (dx * dx + dy * dy <= radius * radius) {
          vis = bv_visible;
        } else if (vis == bv_visible) {
          vis = bv_fog;
        }
      }
    }
  }

public:
  constexpr map(quack::renderer *r) : m_blocks{r} {}

  [[nodiscard]] constexpr auto level() const noexcept { return m_level; }
  [[nodiscard]] constexpr sprite<block_type> &at(unsigned x,
                                                 unsigned y) noexcept {
    return m_blocks.at(x, y).type;
  }
  [[nodiscard]] constexpr sprite<block_type> at(unsigned x,
                                                unsigned y) const noexcept {
    return m_blocks.at(x, y).type;
  }

  void fill_quack(map_coord pc, unsigned d) noexcept {
    update_rogueview(pc, d);

    m_blocks.fill_uv(
        [](auto &i) { return i.type ? i.type->id.uv() : quack::uv{}; });
    m_blocks.fill_colour([](const block &blk) { return quack::colour{}; });
    m_blocks.fill_mult([](const block &blk) {
      switch (blk.vis) {
      case bv_visible:
        return quack::colour{1, 1, 1, 1};
      case bv_none:
        return quack::colour{1, 1, 1, 0};
      default:
        return quack::colour{1, 1, 1, 0.8f};
      }
    });
  }

  void set_level(unsigned l) noexcept {
    m_level = l;

    for (auto &b : m_blocks.data()) {
      b = {sprite{&dot}};
    }

    for (auto x = 0; x < map_width; x++) {
      at(x, 0) = sprite{&hash};
      at(x, map_height - 1) = sprite{&hash};
    }
    for (int y = 0; y < map_height; y++) {
      at(0, y) = sprite{&hash};
      at(map_width - 1, y) = sprite{&hash};
    }
    if (m_level == max_level + 1)
      return;

    static constexpr const auto max_cell_sizes = 5;
    static constexpr const cell_size cell_sizes[max_cell_sizes] = {
        {7, 7}, {7, 5}, {5, 5}, {5, 3}, {3, 3},
    };
    m_cell = cell_sizes[cno::random(max_cell_sizes)];

    subdivide_wide(1, 1, map_width - 2, map_height - 2);

    if ((m_level % 2) == 1) {
      at(map_width - 2, map_height - 2) = sprite{&gt};
    } else {
      at(1, map_height - 2) = sprite{&gt};
    }
  }

  void process_event(const casein::event &e) noexcept {
    m_blocks.process_event(e);
  }
};
} // namespace cno
