export module ecs:ec;
import pog;

namespace ecs::c {
struct marker {};

struct sprite {
  char id;
  float alpha;
  bool seen;
  int layer;
};
} // namespace ecs::c

namespace ecs {
export constexpr const auto map_width = 30;
export constexpr const auto map_height = 20;

export constexpr const auto max_entities = map_width * map_height * 2;

export struct ec {
  pog::entity_list<max_entities> e;

  pog::sparse_set<unsigned, max_entities> armour;
  pog::sparse_set<c::marker, max_entities> bags;
  pog::grid<map_width, map_height, max_entities> blockers;
  pog::sparse_set<pog::grid_coord, max_entities> coords;
  pog::sparse_set<c::marker, max_entities> enemies;
  pog::singleton<c::marker> exit;
  pog::sparse_set<unsigned, max_entities> foods;
  pog::sparse_set<c::marker, max_entities> hostiles;
  pog::sparse_set<c::marker, max_entities> in_use;
  pog::sparse_set<unsigned, max_entities> lights;
  pog::sparse_set<c::marker, max_entities> mobs;
  pog::sparse_set<c::marker, max_entities> non_hostiles;
  pog::singleton<c::marker> player;
  pog::sparse_set<c::sprite, max_entities> sprites;
  pog::sparse_set<c::marker, max_entities> usables;
  pog::sparse_set<c::marker, max_entities> walls;
  pog::sparse_set<unsigned, max_entities> weapons;
};
} // namespace ecs
