export module roll:itemlevel;
import :random;
import ecs;
import inv;

namespace roll {
static constexpr const auto items_per_level = ecs::map_height - 2;

void add_opening_item(ecs::ec *ec) {
  switch (random_item_id()) {
  case 0:
    inv::items::add_jar(ec);
    break;
  case 1:
    inv::items::add_coffer(ec);
    break;
  case 2:
    inv::items::add_torch(ec);
    break;
  default:
    break;
  }
}

void add_midgame_item(ecs::ec *ec) {
  switch (random_item_id()) {
  case 0:
    inv::items::add_jar(ec);
    break;
  case 1:
    inv::items::add_doubleaxe(ec);
    break;
  case 2:
    inv::items::add_coffer(ec);
    break;
  case 3:
    inv::items::add_torch(ec);
    break;
  default:
    break;
  }
}

void add_endgame_item(ecs::ec *ec) {
  switch (random_item_id()) {
  case 0:
    inv::items::add_jar(ec);
    break;
  case 1:
    inv::items::add_coffer(ec);
    break;
  case 2:
    inv::items::add_spear(ec);
    break;
  case 3:
    inv::items::add_shield(ec);
    break;
  case 4:
    inv::items::add_torch(ec);
    break;
  default:
    break;
  }
}

export void add_level_items(ecs::ec *ec, unsigned lvl) {
  for (auto i = 0; i < items_per_level; i++) {
    if (lvl <= 14) {
      add_opening_item(ec);
    } else if (lvl <= 17) {
      add_midgame_item(ec);
    } else {
      add_endgame_item(ec);
    }
  }
}
} // namespace roll