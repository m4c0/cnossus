export module cno:itemlist;
import :blocktype;
import :item;
import :itemtype;
import :map;
import :random;
import casein;
import quack;

namespace cno {
static constexpr const auto max_items_per_level = map_height * 2;
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

class item_list : quack::instance_layout<item, max_items_per_level> {
  void fill_quack() {
    fill_pos([](const item &i) {
      const auto &c = i.coord();
      return quack::rect{static_cast<float>(c.x), static_cast<float>(c.y), 1,
                         1};
    });
    fill_colour([](const item &i) {
      float a = i.type() == nullptr ? 0.0 : 1.0;
      return quack::colour{0, 0, 0, a};
    });
  }

  void resize(unsigned w, unsigned h) override {
    batch()->resize(map_width, map_height, w, h);
  }

public:
  explicit item_list(quack::renderer *r) : instance_layout{r} {}

  void create_for_map(const cno::map *map) noexcept {
    const auto &cur_lvl_items = item_roll_per_level.items[map->level() - 1];
    reset_grid();

    map_coord c{};
    for (c.y = 1; c.y < map_height - 2; c.y++) {
      const auto *type = cur_lvl_items[cno::random(max_item_roll)];
      if (type == nullptr) {
        continue;
      }

      do {
        c.x = cno::random(map_width);
      } while (!map->at(c.x, c.y)->can_walk());

      at(c.y) = {type, c};
    }

    fill_quack();
  }

  void add_item(item new_i) {
    for (auto &i : data()) {
      if (i.type() != nullptr) {
        continue;
      }

      i = new_i;
      break;
    }
  }

  const item_type *fetch(map_coord c) {
    for (auto &i : data()) {
      if (i.type() == nullptr) {
        continue;
      }
      if (i.coord() != c) {
        continue;
      }
      auto res = i.type();
      i = {};
      return res;
    }
    return nullptr;
  }

  using instance_layout::process_event;
};
} // namespace cno
