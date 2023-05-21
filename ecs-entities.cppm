export module ecs:entities;
import :ec;
import pog;

namespace ecs {
export void add_enemy(ecs::ec *ec, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.set(e, c);
}
export void add_item(ecs::ec *ec, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.set(e, c);
}
export void add_player(ecs::ec *ec, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.set(e, c);
}
export void add_rigid_block(ecs::ec *ec, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.set(e, c);
}
} // namespace ecs
