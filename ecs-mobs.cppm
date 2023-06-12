export module ecs:mobs;
import :ec;
import :entities;
import :items;
import :loot;
import pog;
import spr;
import rng;

namespace ecs::mobs {
auto add_loot(ec *ec, pog::eid e, c::loot a, c::loot b) {
  loot_table<c::loot, 3> lt{a, b};
  lt[0] = lt[1] = 1;
  lt[2] = 2;
  return ecs::add_loot(ec, e, lt.pick());
}
} // namespace ecs::mobs

export namespace ecs {
auto add_player(ec *ec) {
  c::sprite s{.id = spr::minotaur, .layer = 1};
  auto e = add_mob(ec, s, {1, 1}, {.life = 20, .dice = 10});
  ec->player.set(e, {});
  return e;
}

// Poisons + Non-hostile

auto add_snake(ec *ec) {
  auto e = add_non_hostile_enemy(ec, spr::snake, {.life = 8});
  ec->poisoners.add(e, 4);
  return mobs::add_loot(ec, e, &add_rawmeat, &add_armguard);
}
auto add_spider(ec *ec) {
  auto e = add_non_hostile_enemy(ec, spr::spider, {.life = 4});
  ec->poisoners.add(e, 4);
  return e;
}
auto add_scorpion(ec *ec) {
  auto e = add_non_hostile_enemy(ec, spr::scorpion, {.life = 4});
  ec->poisoners.add(e, 8);
  return e;
}
auto add_centipede(ec *ec) {
  auto e = add_non_hostile_enemy(ec, spr::centipede, {.life = 6});
  ec->poisoners.add(e, 8);
  return e;
}

// Non-hostile

auto add_rat(ec *ec) {
  auto e = add_non_hostile_enemy(ec, spr::rat, {.life = 6});
  return e;
}

// Poisons + Hostiles

auto add_chimera(ec *ec) {
  auto e = add_hostile_enemy(ec, spr::chimera, {.life = 12});
  ec->poisoners.add(e, 10);
  return e;
}

// Hostiles

auto add_cerberus(ec *ec) {
  auto e = add_hostile_enemy(ec, spr::cerberus, {.life = 8});
  return e;
}
auto add_harpy(ec *ec) {
  auto e = add_hostile_enemy(ec, spr::harpy, {.life = 8});
  return e;
}
auto add_manticore(ec *ec) {
  auto e = add_hostile_enemy(ec, spr::manticore, {.life = 12});
  return e;
}
auto add_croc(ec *ec) {
  auto e = add_hostile_enemy(ec, spr::crocodile, {.life = 20});
  return e;
}
auto add_drakon(ec *ec) {
  auto e = add_hostile_enemy(ec, spr::drakon, {.life = 20});
  return e;
}
auto add_boar(ec *ec) {
  auto e = add_hostile_enemy(ec, spr::boar, {.life = 8});
  return e;
}
auto add_griffin(ec *ec) {
  auto e = add_hostile_enemy(ec, spr::griffin, {.life = 12});
  return e;
}
auto add_sphinx(ec *ec) {
  auto e = add_hostile_enemy(ec, spr::sphinx, {.life = 12});
  return e;
}
auto add_bull(ec *ec) {
  auto e = add_hostile_enemy(ec, spr::bull, {.life = 12});
  return e;
}
} // namespace ecs
