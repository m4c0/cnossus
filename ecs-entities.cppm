export module ecs:entities;
import :ec;
import :random;
import pog;

namespace ecs {
[[nodiscard]] auto find_empty_location(ecs::ec *ec) noexcept {
  pog::grid_coord c;
  do {
    c.x = random(map_width);
    c.y = random(map_height);
  } while (ec->blockers.has(c));
  return c;
}

export auto add_enemy(ecs::ec *ec) {
  auto e = ec->e.alloc();
  auto c = find_empty_location(ec);
  ec->blockers.put(e, c);
  ec->coords.add(e, c);
  return e;
}
export auto add_item(ecs::ec *ec) {
  auto e = ec->e.alloc();
  auto c = find_empty_location(ec);
  ec->coords.add(e, c);
  ec->usables.add(e, {});
  return e;
}
export void add_player(ecs::ec *ec, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.put(e, c);
  ec->coords.add(e, c);
  ec->player.set(e, {});
}

export void add_rigid_block(ecs::ec *ec, char id, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.put(e, c);
  ec->coords.add(e, c);
  ec->sprites.add(e, {id, 1});
}
export void add_walkable_block(ecs::ec *ec, char id, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->coords.add(e, c);
  ec->sprites.add(e, {id, 1});
}
export void add_exit(ecs::ec *ec, char id, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->coords.add(e, c);
  ec->exit.set(e, {});
  ec->sprites.add(e, {id, 1});
  ec->usables.add(e, {});
}
} // namespace ecs
