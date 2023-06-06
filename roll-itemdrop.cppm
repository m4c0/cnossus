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
  loot_table<fn_t, 8> lt{};
  lt[7] = 60;

  lt.pick([ec, c](auto fn) { new_item(ec, c, fn); });
}
} // namespace roll
