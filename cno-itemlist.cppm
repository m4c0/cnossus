export module cno:itemlist;
import :item;
import :itemtype;
import :random;
import casein;
import quack;

namespace cno {
static constexpr const auto max_items_per_level = map_height * 2;
static constexpr const auto max_item_roll = 5;

using item_roll_t = rnd_roll_per_level<const item_type *, max_item_roll>;
static constexpr const item_roll_t item_roll_per_level = {
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &coffer, &torch},
    item_roll_t::roll_t{&jar, &doubleaxe, &coffer, &torch},
    item_roll_t::roll_t{&jar, &doubleaxe, &coffer, &torch},
    item_roll_t::roll_t{&jar, &doubleaxe, &coffer, &torch},
    item_roll_t::roll_t{&jar, &spear, &coffer, &shield, &torch},
    item_roll_t::roll_t{&jar, &spear, &coffer, &shield, &torch},
    item_roll_t::roll_t{&jar, &spear, &coffer, &shield, &torch},
};

class item_list : quack::instance_layout<item, max_items_per_level> {
  void resize(unsigned w, unsigned h) override {
    batch()->resize(map_width, map_height, w, h);
  }

public:
  explicit item_list(quack::renderer *r) : instance_layout{r} {}

  using instance_layout::reset_grid;

  void add_item(item new_i) {
    for (auto &i : data()) {
      if (i)
        continue;

      i = new_i;
      break;
    }
  }

  const item_type *fetch(map_coord c) {
    for (auto &i : data()) {
      if (!i) {
        continue;
      }
      if (i.coord() != c) {
        continue;
      }
      return i.fetch();
    }
    return nullptr;
  }

  void fill_quack(map_coord pc, unsigned d) noexcept {
    const auto &[px, py] = pc;

    fill_pos([](const item &i) {
      const auto &c = i.coord();
      return quack::rect{static_cast<float>(c.x), static_cast<float>(c.y), 1,
                         1};
    });
    fill_colour([px, py, d](const item &i) {
      if (!i)
        return quack::colour{0, 0, 0, 0};

      const auto &[x, y] = i.coord();
      auto dx = x - px;
      auto dy = y - py;

      auto c = i.character();
      auto g = static_cast<float>(c % 16) / 16.0f;
      auto b = static_cast<float>(c / 16) / 16.0f;
      auto a = ((dx * dx) + (dy * dy) <= d * d) ? 1.0f : 0.3f;
      return quack::colour{0, g, b, a};
    });
  }

  using instance_layout::process_event;
};
} // namespace cno
