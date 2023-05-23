export module ecs:ec;
import :compos;
import pog;

namespace ecs {
export constexpr const auto map_width = 30;
export constexpr const auto map_height = 20;

export constexpr const auto max_entities = map_width * map_height * 2;

export struct ec {
  pog::entity_list<max_entities> e;

  pog::grid<map_width, map_height, max_entities> blockers;
  pog::sparse_set<pog::grid_coord, max_entities> coords;
  pog::sparse_set<c::marker, max_entities> enemies;
  pog::singleton<c::marker> exit;
  pog::sparse_set<c::marker, max_entities> hostiles;
  pog::sparse_set<c::marker, max_entities> mobs;
  pog::sparse_set<c::marker, max_entities> non_hostiles;
  pog::singleton<c::marker> player;
  pog::sparse_set<c::sprite, max_entities> sprites;
  pog::sparse_set<c::marker, max_entities> usables;
  pog::grid<map_width, map_height, max_entities> walls;
};

export void wipeout_entity(ec *ec, pog::eid id) {
  ec->blockers.remove(id);
  ec->coords.remove(id);
  ec->enemies.remove(id);
  ec->exit.remove(id);
  ec->hostiles.remove(id);
  ec->mobs.remove(id);
  ec->non_hostiles.remove(id);
  ec->player.remove(id);
  ec->sprites.remove(id);
  ec->usables.remove(id);
  ec->walls.remove(id);

  ec->e.dealloc(id);
}
} // namespace ecs
