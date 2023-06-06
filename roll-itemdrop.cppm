export module roll:itemdrop;
import :random;
import ecs;
import inv;
import pog;

namespace roll {
using fn_t = decltype(inv::items::add_driedfruit) *;

inline void new_item(ecs::ec *ec, pog::grid_coord c, fn_t fn) {
  auto e = fn(ec);
  ec->coords.update(e, c);
  ec->usables.update(e, c);
}

void add_jar_drop(ecs::ec *ec, pog::grid_coord c, int lvl) {
  loot_table<fn_t, 4> lt{
      &inv::items::add_driedfruit,
      &inv::items::add_candle,
      &inv::items::add_cheese,
  };
  lt[0] = lt[1] = 30;
  lt[2] = 30 - lvl;
  lt[3] = 60;

  lt.pick([ec, c](auto fn) { new_item(ec, c, fn); });
}
void add_chest_drop(ecs::ec *ec, pog::grid_coord c, int lvl) {
  constexpr const fn_t armour[7]{
      &inv::items::add_cloth,    &inv::items::add_leather,
      &inv::items::add_greave,   &inv::items::add_armguard,
      &inv::items::add_pauldron, &inv::items::add_leather,
      &inv::items::add_scale,
  };
  constexpr const fn_t weapons[5]{
      &inv::items::add_knife, &inv::items::add_sickle, &inv::items::add_adze,
      &inv::items::add_axe,   &inv::items::add_sword,
  };
  // TODO: expose max_levels and set these based on it
  loot_table<fn_t, 5> lt{
      armour[lvl / 3],
      weapons[lvl / 4],
      &inv::items::add_bag,
      &inv::items::add_oillamp,
  };
  lt[0] = lt[1] = lt[2] = lt[3] = 1;
  lt[4] = 2;

  lt.pick([ec, c](auto fn) { new_item(ec, c, fn); });
}
} // namespace roll
