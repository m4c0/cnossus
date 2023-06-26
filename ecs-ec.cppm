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
  pog::entity_list e{max_entities};

  pog::sparse_set<unsigned> armour{max_entities};
  pog::sparse_set<c::marker> bags{max_entities};
  pog::grid<map_width, map_height, max_entities> blockers;
  pog::sparse_set<pog::grid_coord> coords{max_entities};
  pog::sparse_set<c::marker> enemies{max_entities};
  pog::singleton<c::marker> exit;
  pog::sparse_set<unsigned> foods{max_entities};
  pog::sparse_set<c::marker> hostiles{max_entities};
  pog::sparse_set<c::marker> in_use{max_entities};
  pog::sparse_set<unsigned> lights{max_entities};
  pog::sparse_set<c::loot> loot{max_entities};
  pog::sparse_set<c::mob> mobs{max_entities};
  pog::sparse_set<c::marker> non_hostiles{max_entities};
  pog::singleton<c::marker> player;
  pog::sparse_set<unsigned> poisoners{max_entities};
  pog::sparse_set<c::sprite> sprites{max_entities};
  pog::sparse_set<c::marker> usables{max_entities};
  pog::sparse_set<c::marker> walls{max_entities};
  pog::sparse_set<unsigned> weapons{max_entities};
};
} // namespace ecs
