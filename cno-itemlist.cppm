export module cno:itemlist;
import :itemtype;
import :random;
import casein;
import quack;

namespace cno {
static constexpr const auto max_items_per_level = map_height * 2;
static constexpr const auto max_item_roll = 5;

struct item {
  sprite<item_type> type;
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

class item_list : sbatch<item, max_items_per_level> {
public:
  using sbatch::sbatch;

  using sbatch::add;
  using sbatch::reset_grid;

  sprite<item_type> fetch(map_coord c) {
    for (auto &i : data()) {
      if (!i.type)
        continue;

      if (i.coord != c) {
        continue;
      }
      auto res = i.type;
      i.type = {};
      return res;
    }
    return {};
  }

  void fill_quack(map_coord pc, unsigned d) noexcept {
    const auto &[px, py] = pc;

    fill_pos([](const item &i) {
      const auto &c = i.coord;
      return quack::rect{static_cast<float>(c.x), static_cast<float>(c.y), 1,
                         1};
    });
    fill_uv(
        [](const item &i) { return i.type ? i.type->id.uv() : quack::uv{}; });
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
