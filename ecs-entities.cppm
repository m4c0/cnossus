export module ecs:entities;
import :ec;
import :random;
import pog;

namespace ecs {
[[nodiscard]] auto find_empty_location(ec *ec) noexcept {
  pog::grid_coord c;
  do {
    c.x = random(map_width);
    c.y = random(map_height);
  } while (ec->blockers.has(c));
  return c;
}

auto add_mob(ec *ec, char id, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.put(e, c);
  ec->coords.add(e, c);
  ec->mobs.add(e, {});
  ec->sprites.add(e, {id});
  return e;
}

auto add_enemy(ec *ec, char id) {
  auto c = find_empty_location(ec);
  auto e = add_mob(ec, id, c);
  ec->enemies.add(e, {});
  return e;
}
export auto add_hostile_enemy(ec *ec, char id) {
  auto e = add_enemy(ec, id);
  ec->hostiles.add(e, {});
  return e;
}
export auto add_non_hostile_enemy(ec *ec, char id) {
  auto e = add_enemy(ec, id);
  ec->non_hostiles.add(e, {});
  return e;
}

export void set_mob_position(ec *ec, auto id, pog::grid_coord c) {
  ec->blockers.update(id, c);
  ec->coords.update(id, c);
}

auto add_item(ec *ec, char id) {
  auto e = ec->e.alloc();
  auto c = find_empty_location(ec);
  ec->coords.add(e, c);
  ec->usables.add(e, {});
  ec->sprites.add(e, {id});
  return e;
}
export auto add_bag_item(ec *ec, char id) {
  auto e = add_item(ec, id);
  ec->bags.add(e, {});
  return e;
}
export auto add_container_item(ec *ec, char id) {
  auto e = add_item(ec, id);
  // TODO: add a component to hold the loot table
  return e;
}
export auto add_weapon_item(ec *ec, char id, unsigned pwr) {
  auto e = add_item(ec, id);
  ec->weapons.add(e, pwr);
  return e;
}
export auto add_armour_item(ec *ec, char id, unsigned pwr) {
  auto e = add_item(ec, id);
  ec->armour.add(e, pwr);
  return e;
}
export auto add_food_item(ec *ec, char id, unsigned life) {
  auto e = add_item(ec, id);
  ec->foods.add(e, life);
  return e;
}
export auto add_light_item(ec *ec, char id, unsigned timer) {
  auto e = add_item(ec, id);
  ec->lights.add(e, timer);
  return e;
}

export auto add_player(ec *ec, char id, pog::grid_coord c) {
  auto e = add_mob(ec, id, c);
  ec->player.set(e, {});
  return e;
}

constexpr auto add_block(ec *ec, char id, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->coords.add(e, c);
  ec->sprites.add(e, {id});
  return e;
}
export constexpr void add_rigid_block(ec *ec, char id, pog::grid_coord c) {
  auto e = add_block(ec, id, c);
  ec->blockers.put(e, c);
  ec->walls.put(e, c);
}
export constexpr void add_walkable_block(ec *ec, char id, pog::grid_coord c) {
  add_block(ec, id, c);
}
export constexpr void add_exit(ec *ec, char id, pog::grid_coord c) {
  auto e = add_block(ec, id, c);
  ec->exit.set(e, {});
  ec->usables.add(e, {});
}
export constexpr void remove_rigid_wall(ec *ec, pog::grid_coord c) {
  auto id = ec->blockers.get(c);
  if (!id)
    return;

  ec->blockers.remove(id);
  ec->coords.remove(id);
  ec->sprites.remove(id);
  ec->walls.remove(id);
}
} // namespace ecs
