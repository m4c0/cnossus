export module roll:itemdrop;
import :random;
import ecs;
import inv;
import pog;

namespace roll {
using fn_t = decltype(ecs::add_driedfruit) *;

inline void new_item(ecs::ec *ec, pog::grid_coord c, fn_t fn) {
  auto e = fn(ec);
  ec->coords.update(e, c);
}

void add_jar_drop(ecs::ec *ec, pog::grid_coord c, int lvl) {
  loot_table<fn_t, 4> lt{
      &ecs::add_driedfruit,
      &ecs::add_candle,
      &ecs::add_cheese,
  };
  lt[0] = lt[1] = 30;
  lt[2] = 30 - lvl;
  lt[3] = 60;

  lt.pick([ec, c](auto fn) { new_item(ec, c, fn); });
}
void add_chest_drop(ecs::ec *ec, pog::grid_coord c, int lvl) {
  constexpr const fn_t armour[7]{
      &ecs::add_cloth,    &ecs::add_leather,  &ecs::add_greave,
      &ecs::add_armguard, &ecs::add_pauldron, &ecs::add_leather,
      &ecs::add_scale,
  };
  constexpr const fn_t weapons[5]{
      &ecs::add_knife, &ecs::add_sickle, &ecs::add_adze,
      &ecs::add_axe,   &ecs::add_sword,
  };
  // TODO: expose max_levels and set these based on it
  loot_table<fn_t, 5> lt{
      armour[lvl / 3],
      weapons[lvl / 4],
      &ecs::add_bag,
      &ecs::add_oillamp,
  };
  lt[0] = lt[1] = lt[2] = lt[3] = 1;
  lt[4] = 2;

  lt.pick([ec, c](auto fn) { new_item(ec, c, fn); });
}
} // namespace roll
