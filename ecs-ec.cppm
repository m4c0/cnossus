export module ecs:ec;
import pog;
import spr;

namespace ecs {
export struct ec;
}
namespace ecs::c {
struct marker {};

struct sprite {
  spr::id id;
  float alpha;
  bool seen;
  int layer;
};
struct mob {
  unsigned life;
  unsigned attack = life;
  unsigned defense = life;
};

using loot = pog::eid (*)(ec *);
} // namespace ecs::c

namespace ecs {
export constexpr const auto map_width = 30;
export constexpr const auto map_height = 20;

export constexpr const auto max_entities = map_width * map_height * 2;

export struct ec {
  pog::entity_list e{};

  pog::sparse_set<unsigned> armour{};
  pog::sparse_set<c::marker> bags{};
  pog::grid<map_width, map_height, max_entities> blockers;
  pog::sparse_set<pog::grid_coord> coords{};
  pog::sparse_set<c::marker> enemies{};
  pog::singleton<c::marker> exit;
  pog::sparse_set<unsigned> foods{};
  pog::sparse_set<c::marker> hostiles{};
  pog::sparse_set<c::marker> in_use{};
  pog::sparse_set<unsigned> lights{};
  pog::sparse_set<c::loot> loot{};
  pog::sparse_set<c::mob> mobs{};
  pog::sparse_set<c::marker> non_hostiles{};
  pog::singleton<c::marker> player;
  pog::sparse_set<unsigned> poisoners{};
  pog::sparse_set<c::sprite> sprites{};
  pog::sparse_set<c::marker> usables{};
  pog::sparse_set<c::marker> walls{};
  pog::sparse_set<unsigned> weapons{};
};
} // namespace ecs
