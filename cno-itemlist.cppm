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
  using sbatch::fill_quack;
  using sbatch::process_event;
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
};
} // namespace cno
