export module ecs:ec;
import :compos;
import pog;

namespace ecs {
export constexpr const auto map_width = 30;
export constexpr const auto map_height = 20;
static constexpr const auto max_entities = map_width * map_height * 2;

export struct ec {
  pog::entity_list<max_entities> e;
  pog::grid<map_width, map_height, max_entities> blockers;
  pog::sparse_set<pog::grid_coord, max_entities> coords;
  pog::sparse_set<ecs::c::exit, 1> exit;
};
} // namespace ecs
