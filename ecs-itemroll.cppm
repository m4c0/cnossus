export module ecs:itemroll;
import :containers;
import :ec;
import :items;
import rng;

namespace ecs {
void add_opening_item(ec *ec, unsigned lvl) {
  switch (rng::rand(5)) {
  case 0:
    add_jar(ec, lvl);
    break;
  case 1:
    add_coffer(ec, lvl);
    break;
  case 2:
    add_torch(ec);
    break;
  default:
    break;
  }
}

void add_midgame_item(ec *ec, unsigned lvl) {
  switch (rng::rand(5)) {
  case 0:
    add_jar(ec, lvl);
    break;
  case 1:
    add_doubleaxe(ec);
    break;
  case 2:
    add_coffer(ec, lvl);
    break;
  case 3:
    add_torch(ec);
    break;
  default:
    break;
  }
}

void add_endgame_item(ec *ec, unsigned lvl) {
  switch (rng::rand(5)) {
  case 0:
    add_jar(ec, lvl);
    break;
  case 1:
    add_coffer(ec, lvl);
    break;
  case 2:
    add_spear(ec);
    break;
  case 3:
    add_shield(ec);
    break;
  case 4:
    add_torch(ec);
    break;
  default:
    break;
  }
}

static constexpr const auto items_per_level = map_height - 2;
export void add_level_items(ec *ec, unsigned lvl) {
  for (auto i = 0; i < items_per_level; i++) {
    if (lvl <= 14) {
      add_opening_item(ec, lvl);
    } else if (lvl <= 17) {
      add_midgame_item(ec, lvl);
    } else {
      add_endgame_item(ec, lvl);
    }
  }
}
} // namespace ecs
