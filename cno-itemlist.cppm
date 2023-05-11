export module cno:itemlist;
import :itemtype;
import :random;
import casein;
import quack;

namespace cno {
static constexpr const auto max_items_per_level = map_height * 2;
static constexpr const auto max_item_roll = 5;

using item = sprite<item_type>;

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

using item_list = sbatch<item, max_items_per_level>;
} // namespace cno
