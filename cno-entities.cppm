export module cno:entities;
import :objects;
import :random;
import jute;
import map;
import qsu;

namespace cno {
enum carry_type {
  carry_one,
  carry_two,
  carry_many,
};
struct inventory_pos {
  unsigned sec;
  unsigned row;
};

class item_type;
static constexpr const auto max_item_drops = 5;
using item_loot_table = rnd_roll_per_level<item_type, max_item_drops>;

struct item_type {
  jute::view name;
  qsu::id id;
  int attack{};
  int defense{};
  carry_type carry{carry_many};
  unsigned life_gain{};
  int light_provided{};
  const item_loot_table *drops{};
};

enum hostilities { h_none, h_scaried, h_aggresive };

static constexpr const auto max_mob_drops = 3;
using mob_drops = rnd_roll<item_type, max_mob_drops>;
struct mob_type {
  jute::view name;
  qsu::id id;
  unsigned life;
  unsigned dice = life;
  hostilities hostility;
  unsigned poison{};
  mob_drops drops{};
};

struct bonus {
  int attack;
  int defense;
  int damage;
};
struct mob : qsu::sprite<mob_type> {
  static constexpr const auto initial_max_actions = 20;

  ranged life{type ? type->life : 0};
  ranged actions{initial_max_actions};
  unsigned poison{};
  float damage_timer{};
  bonus bonus{};
};
[[nodiscard]] constexpr auto is_player(const mob &a) noexcept {
  return a.type->hostility == h_none;
}

using item_list =
    qsu::layout2<qsu::sprite<item_type>, ::map::width, ::map::height>;
using mob_list = qsu::layout2<mob, ::map::width, ::map::height>;
} // namespace cno
