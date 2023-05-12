export module cno:entities;
import :blocktype;
import :globals;
import :itemtype;
import :mobtype;
import :sprite;

namespace cno {
static constexpr const auto max_items_per_level = map_height * 2;

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

using item_list = sbatch<item, max_items_per_level>;
using map = sbatch<block, map_width * map_height>;
using mob_list = sbatch<mob, max_mobs_per_level>;
} // namespace cno
