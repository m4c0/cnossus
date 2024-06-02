export module ecs:containers;
import :ec;
import :entities;
import :items;
import :loot;
import pog;
import spr;

export namespace ecs {
auto add_jar(ecs::ec *ec, unsigned lvl) {
  loot_table<4> lt{
      &ecs::add_driedfruit,
      &ecs::add_candle,
      &ecs::add_cheese,
  };
  lt[0] = lt[1] = 30;
  lt[2] = 30 - lvl;
  lt[3] = 60;

  auto e = add_container_item(ec, spr::jar);
  return add_loot(ec, e, lt.pick());
}

// chest
auto add_coffer(ecs::ec *ec, unsigned lvl) {
  constexpr const c::loot armour[7]{
      &ecs::add_cloth,    &ecs::add_leather,  &ecs::add_greave,
      &ecs::add_armguard, &ecs::add_pauldron, &ecs::add_leather,
      &ecs::add_scale,
  };
  constexpr const c::loot weapons[5]{
      &ecs::add_knife, &ecs::add_sickle, &ecs::add_adze,
      &ecs::add_axe,   &ecs::add_sword,
  };
  // TODO: expose max_levels and set these based on it
  loot_table<5> lt{
      armour[lvl / 3],
      weapons[lvl / 4],
      &ecs::add_bag,
      &ecs::add_oillamp,
  };
  lt[0] = lt[1] = lt[2] = lt[3] = 1;
  lt[4] = 2;

  auto e = add_container_item(ec, spr::coffer);
  return add_loot(ec, e, lt.pick());
}
} // namespace ecs
