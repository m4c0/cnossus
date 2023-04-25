export module cno:itemlist;
import :item;
import :itemtype;
import :random;
import quack;

namespace cno {
static constexpr const auto max_items_per_level = 20;
static constexpr const auto max_item_roll = 5;

static constexpr const struct {
  const item_type *items[max_level][max_item_roll];
} item_roll_per_level = {{
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &coffer, &torch},
    {&jar, &doubleaxe, &coffer, &torch},
    {&jar, &doubleaxe, &coffer, &torch},
    {&jar, &doubleaxe, &coffer, &torch},
    {&jar, &spear, &coffer, &shield, &torch},
    {&jar, &spear, &coffer, &shield, &torch},
    {&jar, &spear, &coffer, &shield, &torch},
}};

class item_list {
  quack::instance_layout<item, max_items_per_level> m_batch;

public:
  void set_level(unsigned l) noexcept {}
};
} // namespace cno
