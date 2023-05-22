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

auto add_mob(ecs::ec *ec, char id, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.put(e, c);
  ec->coords.add(e, c);
  ec->enemies.add(e, {});
  ec->sprites.add(e, {id});
  return e;
}

export auto add_enemy(ecs::ec *ec, char id) {
  auto c = find_empty_location(ec);
  auto e = add_mob(ec, id, c);
  ec->enemies.add(e, {});
  return e;
}
export auto add_item(ecs::ec *ec, char id) {
  auto e = ec->e.alloc();
  auto c = find_empty_location(ec);
  ec->coords.add(e, c);
  ec->usables.add(e, {});
  ec->sprites.add(e, {id});
  return e;
}
export auto add_player(ecs::ec *ec, char id, pog::grid_coord c) {
  auto e = add_mob(ec, id, c);
  ec->player.set(e, {});
  return e;
}

export void add_rigid_block(ecs::ec *ec, char id, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.put(e, c);
  ec->coords.add(e, c);
  ec->sprites.add(e, {id});
}
export void add_walkable_block(ecs::ec *ec, char id, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->coords.add(e, c);
  ec->sprites.add(e, {id});
}
export void add_exit(ecs::ec *ec, char id, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->coords.add(e, c);
  ec->exit.set(e, {});
  ec->sprites.add(e, {id});
  ec->usables.add(e, {});
}
} // namespace ecs
