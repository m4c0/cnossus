export module ecs:entities;
import :ec;
import :random;
import pog;
import spr;

namespace ecs {
[[nodiscard]] auto find_empty_location(ec *ec) noexcept {
  pog::grid_coord c;
  do {
    c.x = random(map_width);
    c.y = random(map_height);
  } while (ec->blockers.has(c));
  return c;
}

auto add_mob(ec *ec, c::sprite s, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->blockers.put(e, c);
  ec->coords.add(e, c);
  ec->mobs.add(e, {});
  ec->sprites.add(e, s);
  return e;
}

auto add_enemy(ec *ec, spr::id id) {
  auto c = find_empty_location(ec);
  auto e = add_mob(ec, {id}, c);
  ec->enemies.add(e, {});
  return e;
}
export auto add_hostile_enemy(ec *ec, spr::id id) {
  auto e = add_enemy(ec, id);
  ec->hostiles.add(e, {});
  return e;
}
export auto add_non_hostile_enemy(ec *ec, spr::id id) {
  auto e = add_enemy(ec, id);
  ec->non_hostiles.add(e, {});
  return e;
}

export void set_mob_position(ec *ec, pog::eid id, pog::grid_coord c) {
  ec->blockers.update(id, c);
  ec->coords.update(id, c);
}
export void remove_mob(ec *ec, pog::eid id) {
  ec->blockers.remove(id);
  ec->coords.remove(id);
  ec->enemies.remove(id);
  ec->hostiles.remove(id);
  ec->non_hostiles.remove(id);
  ec->player.remove(id);
  ec->mobs.remove(id);
  ec->sprites.remove(id);
  ec->e.dealloc(id);
}

auto add_item(ec *ec, spr::id id) {
  auto e = ec->e.alloc();
  auto c = find_empty_location(ec);
  ec->coords.add(e, c);
  ec->usables.add(e, {});
  ec->sprites.add(e, {id});
  return e;
}
export auto add_bag_item(ec *ec, spr::id id) {
  auto e = add_item(ec, id);
  ec->bags.add(e, {});
  return e;
}
export auto add_container_item(ec *ec, spr::id id) {
  auto e = add_item(ec, id);
  // TODO: add a component to hold the loot table
  return e;
}
export auto add_weapon_item(ec *ec, spr::id id, unsigned pwr) {
  auto e = add_item(ec, id);
  ec->weapons.add(e, pwr);
  return e;
}
export auto add_armour_item(ec *ec, spr::id id, unsigned pwr) {
  auto e = add_item(ec, id);
  ec->armour.add(e, pwr);
  return e;
}
export auto add_food_item(ec *ec, spr::id id, unsigned life) {
  auto e = add_item(ec, id);
  ec->foods.add(e, life);
  return e;
}
export auto add_light_item(ec *ec, spr::id id, unsigned timer) {
  auto e = add_item(ec, id);
  ec->lights.add(e, timer);
  return e;
}

export void remove_item(ecs::ec *ec, pog::eid id) {
  ec->armour.remove(id);
  ec->bags.remove(id);
  ec->coords.remove(id);
  ec->foods.remove(id);
  ec->lights.remove(id);
  ec->sprites.remove(id);
  ec->usables.remove(id);
  ec->weapons.remove(id);
  ec->e.dealloc(id);
}

export auto add_player(ec *ec) {
  c::sprite s{.id = spr::minotaur, .layer = 1};
  auto e = add_mob(ec, s, {1, 1});
  ec->player.set(e, {});
  return e;
}

constexpr auto add_block(ec *ec, c::sprite s, pog::grid_coord c) {
  auto e = ec->e.alloc();
  ec->coords.add(e, c);
  ec->sprites.add(e, s);
  ec->walls.add(e, {});
  return e;
}
export constexpr void add_rigid_block(ec *ec, spr::id id, pog::grid_coord c) {
  auto e = add_block(ec, {id}, c);
  ec->blockers.put(e, c);
}
export constexpr void add_walkable_block(ec *ec, spr::id id,
                                         pog::grid_coord c) {
  c::sprite s{.id = id, .layer = -1};
  auto e = add_block(ec, s, c);
}

export constexpr void add_exit(ec *ec, pog::grid_coord c) {
  auto e = add_block(ec, {spr::exit}, c);
  ec->exit.set(e, {});
}
export constexpr void remove_wall(ec *ec, pog::eid id) {
  ec->blockers.remove(id);
  ec->coords.remove(id);
  ec->sprites.remove(id);
  ec->walls.remove(id);
  ec->e.dealloc(id);
}

export void remove_level(ec *ec) {
  ec->walls.for_each_r([ec](auto, auto id) { remove_wall(ec, id); });
  ec->usables.for_each_r([ec](auto, auto id) { remove_item(ec, id); });
  ec->enemies.for_each_r([ec](auto, auto id) { remove_mob(ec, id); });
  ec->exit.set({}, {});
}
} // namespace ecs
