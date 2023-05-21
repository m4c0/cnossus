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

export void add_enemy(ecs::ec *ec, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.set(e, c);
}
export void add_item(ecs::ec *ec, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.set(e, c);
}
export void add_rigid_block(ecs::ec *ec, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.set(e, c);
}
} // namespace ecs
