export module cno:entities;
import :globals;
import :random;
import :sprite;
import jute;

namespace cno {
struct block_type {
  jute::view name;
  sid id;
  bool can_walk{};
  bool transparent{};
};

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
using item_loot_table = rnd_roll_per_level<const item_type *, max_item_drops>;

struct item_type {
  jute::view name;
  sid id;
  int attack{};
  int defense{};
  carry_type carry{carry_many};
  int life_gain{};
  int light_provided{};
  inventory_pos inv_coords{~0U, ~0U};
  const item_loot_table *drops{};
};

enum hostilities { h_none, h_scaried, h_aggresive };

static constexpr const auto max_mob_drops = 3;
using mob_drops = rnd_roll<const item_type *, max_mob_drops>;
struct mob_type {
  jute::view name;
  sid id;
  unsigned life;
  unsigned dice = life;
  hostilities hostility;
  unsigned poison{};
  mob_drops drops{};
};

using block = sprite<block_type>;
using item = sprite<item_type>;

struct bonus {
  int attack;
  int defense;
  int damage;
};
struct mob : sprite<mob_type> {
  static constexpr const auto initial_max_actions = 20;

  ranged life{type ? type->life : 0};
  ranged actions{initial_max_actions};
  unsigned poison{};
  float damage_timer{};
  bonus bonus{};
};

static constexpr const auto max_items_per_level = map_height * 2;
using item_list = sbatch<item, max_items_per_level>;

using map = sbatch<block, map_width * map_height>;

static constexpr const auto max_mobs_per_level = map_height;
using mob_list = sbatch<mob, max_mobs_per_level>;
} // namespace cno
