export module cno:itemlist;
import :blocktype;
import :item;
import :itemtype;
import :map;
import :random;
import quack;

namespace cno {
static constexpr const auto max_items_per_level = map_height;
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
  void create_for_map(const cno::map *map) noexcept {
    const auto &cur_lvl_items = item_roll_per_level.items[map->level() - 1];
    m_batch.reset_grid();

    map_coord c{};
    for (c.y = 1; c.y < map_height - 2; c.y++) {
      const auto *type = cur_lvl_items[cno::random(max_item_roll)];
      if (!type) {
        continue;
      }

      do {
        c.x = cno::random(map_width);
      } while (!map->at(c.x, c.y)->can_walk());

      m_batch.at(c.y) = {type, c};
    }
  }
};
} // namespace cno
