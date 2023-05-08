export module cno:itemlist;
import :itemtype;
import :random;
import casein;
import quack;

namespace cno {
static constexpr const auto max_items_per_level = map_height * 2;
static constexpr const auto max_item_roll = 5;

struct item {
  const item_type *type;
  map_coord coord;
};

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
      if (i.type != nullptr)
        continue;

      i = new_i;
      break;
    }
  }

  const item_type *fetch(map_coord c) {
    for (auto &i : data()) {
      if (i.type == nullptr) {
        continue;
      }
      if (i.coord != c) {
        continue;
      }
      auto res = i.type;
      i.type = nullptr;
      return res;
    }
    return nullptr;
  }

  void fill_quack(map_coord pc, unsigned d) noexcept {
    const auto &[px, py] = pc;

    fill_pos([](const item &i) {
      const auto &c = i.coord;
      return quack::rect{static_cast<float>(c.x), static_cast<float>(c.y), 1,
                         1};
    });
    fill_uv([](const item &i) {
      if (i.type == nullptr)
        return quack::uv{};

      auto c = i.type->character;
      auto u = static_cast<float>(c % 16);
      auto v = static_cast<float>(c / 16);

      auto u0 = u / 16.0f;
      auto v0 = v / 16.0f;
      auto u1 = (u + 1.0f) / 16.0f;
      auto v1 = (v + 1.0f) / 16.0f;

      return quack::uv{{u0, v0}, {u1, v1}};
    });
    fill_colour([](const item &i) { return quack::colour{}; });
    fill_mult([px, py, d](const item &i) {
      const auto &[x, y] = i.coord;
      auto dx = x - px;
      auto dy = y - py;
      auto a = ((dx * dx) + (dy * dy) <= d * d) ? 1.0f : 0.3f;
      return quack::colour{1, 1, 1, a};
    });
  }

  using instance_layout::process_event;
};
} // namespace cno
