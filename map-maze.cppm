export module map:maze;
import :block;
import ecs;
import qsu;

extern "C" int rand();

namespace map {
export constexpr const auto width = ecs::map_width;
export constexpr const auto height = ecs::map_height;

[[nodiscard]] auto random(unsigned max) { return rand() % max; }

export class maze {
  struct cell_size {
    unsigned w;
    unsigned h;
  };

  qsu::type<block> m_map[height][width]{};
  cell_size m_cell;

  [[nodiscard]] auto &at(unsigned x, unsigned y) noexcept {
    return m_map[y][x];
  }

  [[nodiscard]] auto random_furniture() const noexcept {
    return qsu::type{random(2) == 0 ? &star : &andsign};
  }

  void furnish_room(unsigned x1, unsigned y1, unsigned x2,
                    unsigned y2) noexcept {
    auto w = x2 - x1 + 1;
    auto h = y2 - y1 + 1;

    if (w == 3 && h == 3) {
      at(x1 + 1, y1 + 1) = random_furniture();
    } else if (w == 3) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        at(x1 + 1, y) = qsu::type{&vbar};
      }
      if ((h % 2) == 0) {
        at(x2 - 1, y2) = random_furniture();
      }
    } else if (h == 3) {
      for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
        at(x, y1 + 1) = qsu::type{&vbar};
      }
      if ((w % 2) == 0) {
        at(x2, y2 - 1) = random_furniture();
      }
    } else if ((w == 4) || (h == 4)) {
      for (auto y = y1 + 1; y <= y2 - 1; y++) {
        for (auto x = x1 + 1; x <= x2 - 1; x++) {
          at(x, y) = qsu::type{&tilda};
        }
      }
    } else if (w == 5) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        at(x1 + 1, y) = qsu::type{&vbar};
        at(x2 - 1, y) = qsu::type{&vbar};
      }
      if ((h % 2) == 0) {
        at(x2 - 2, y2) = random_furniture();
      }
    } else if (h == 5) {
      for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
        at(x, y1 + 1) = qsu::type{&vbar};
        at(x, y2 - 1) = qsu::type{&vbar};
      }
      if ((w % 2) == 0) {
        at(x2, y2 - 2) = random_furniture();
      }
    } else if ((w == 7) && (h == 7)) {
      for (auto y = y1 + 1; y <= y2 - 1; y += 2) {
        for (auto x = x1 + 1; x <= x2 - 1; x += 2) {
          at(x, y) = qsu::type{&vbar};
        }
      }
      at(x1 + 3, y1 + 3) = random_furniture();
    } else if ((w > 2) && (h > 2)) {
      for (auto y = y1; y <= y2; y++) {
        at(x1, y) = at(x2, y) = qsu::type{&comma};
      }
      for (auto x = x1; x <= x2; x++) {
        at(x, y1) = at(x, y2) = qsu::type{&comma};
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
      x += random(w - 2);
    }

    for (auto y = y1; y <= y2; y++) {
      at(x, y) = qsu::type{&hash};
    }

    auto wall_1 = subdivide_high(x1, y1, x - 1, y2);
    auto wall_2 = subdivide_high(x + 1, y1, x2, y2);

    unsigned door_y;
    do {
      door_y = y1 + random(h);
    } while ((door_y == wall_1) || (door_y == wall_2));

    at(x, door_y) = qsu::type{&dot};
    at(x - 1, door_y) = qsu::type{&dot};
    at(x + 1, door_y) = qsu::type{&dot};
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
      y += random(h - 2);
    }

    for (auto x = x1; x <= x2; x++) {
      at(x, y) = qsu::type{&hash};
    }

    auto wall_1 = subdivide_wide(x1, y1, x2, y - 1);
    auto wall_2 = subdivide_wide(x1, y + 1, x2, y2);

    unsigned door_x;
    do {
      door_x = x1 + random(w);
    } while ((door_x == wall_1) || (door_x == wall_2));

    at(door_x, y) = qsu::type{&dot};
    at(door_x, y - 1) = qsu::type{&dot};
    at(door_x, y + 1) = qsu::type{&dot};
    return y;
  }

public:
  void build_level(unsigned lvl) noexcept {
    for (auto &row : m_map) {
      for (auto &blk : row) {
        blk = qsu::type{&dot};
      }
    }

    for (auto x = 0; x < width; x++) {
      at(x, 0) = qsu::type{&hash};
      at(x, height - 1) = qsu::type{&hash};
    }
    for (int y = 0; y < height; y++) {
      at(0, y) = qsu::type{&hash};
      at(width - 1, y) = qsu::type{&hash};
    }

    static constexpr const auto max_cell_sizes = 5;
    static constexpr const cell_size cell_sizes[max_cell_sizes] = {
        {7, 7}, {7, 5}, {5, 5}, {5, 3}, {3, 3},
    };
    m_cell = cell_sizes[random(max_cell_sizes)];

    subdivide_wide(1, 1, width - 2, height - 2);

    if ((lvl % 2) == 1) {
      at(width - 2, height - 2) = qsu::type{&gt};
    } else {
      at(1, height - 2) = qsu::type{&gt};
    }
  }

  void build_sprites(qsu::layout<qsu::sprite<block>, width, height> *tgt) {
    for (auto y = 0U; y < height; y++) {
      for (auto x = 0U; x < width; x++) {
        tgt->at(y * width + x) = {
            .type = m_map[y][x],
            .coord = {x, y},
        };
      }
    }
  }

  // TODO: just add entities in `build level`
  void build_entities(ecs::ec *ec) {
    for (auto y = 0U; y < height; y++) {
      for (auto x = 0U; x < width; x++) {
        if (m_map[y][x]->id == gt.id) {
          ecs::add_exit(ec, {x, y});
          continue;
        }
        if (m_map[y][x]->can_walk) {
          ecs::add_walkable_block(ec, {x, y});
        } else {
          ecs::add_rigid_block(ec, {x, y});
        }
      }
    }
  }
};
} // namespace map
