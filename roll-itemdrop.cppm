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
  switch (lvl) {
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 16:
  case 18:
  case 20:
    switch (random_item_id()) {
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
    break;
  default:
    break;
  }
}
void add_chest_drop(ecs::ec *ec, pog::grid_coord c, int lvl) {}
} // namespace roll
