export module mobs;
import ecs;
import inv;
import pog;

namespace mobs {
export bool move_mob(ecs::ec *ec, pog::eid id, int dx, int dy) {
  auto [x, y] = ec->coords.get(id);
  auto tx = x + dx;
  auto ty = y + dy;

  auto bid = ec->blockers.get({tx, ty});
  if (!bid) {
    ecs::set_mob_position(ec, id, {tx, ty});
    return true;
  }

  if (ec->walls.has(bid))
    return false;

  if (!ec->enemies.has(bid))
    return false;

  ecs::remove_mob(ec, bid);
  return true;
}

export void move_player(ecs::ec *ec, int dx, int dy) {
  auto pid = ec->player.get_id();
  if (move_mob(ec, pid, dx, dy))
    inv::take_items(ec);
}
} // namespace mobs
