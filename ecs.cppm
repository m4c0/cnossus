export module ecs;
import pog;

namespace ecs {
export constexpr const auto map_width = 30;
export constexpr const auto map_height = 20;
static constexpr const auto max_entities = map_width * map_height * 2;

export struct ec {
  pog::entity_list<max_entities> e;
  pog::grid<map_width, map_height, max_entities> blockers;
};

} // namespace ecs
