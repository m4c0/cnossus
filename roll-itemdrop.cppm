export module roll:itemdrop;
import :random;
import ecs;
import inv;
import pog;

namespace roll {
inline void new_item(ecs::ec *ec, pog::grid_coord c, auto &&fn) {
  auto e = fn(ec);
  ec->coords.update(e, c);
  ec->usables.update(e, c);
}

void add_jar_drop(ecs::ec *ec, pog::grid_coord c, int lvl) {
  loot_table<3> lt{};
  lt[0] = lt[1] = 30;
  lt[2] = 30 - lvl;
  lt[3] = 60;
  switch (lt.pick()) {
  case 0:
    new_item(ec, c, inv::items::add_driedfruit);
    break;
  case 1:
    new_item(ec, c, inv::items::add_candle);
    break;
  case 2:
    new_item(ec, c, inv::items::add_cheese);
    break;
  default:
    break;
  }
}
void add_chest_drop(ecs::ec *ec, pog::grid_coord c, int lvl) {}
} // namespace roll
