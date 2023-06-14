export module mobs;
import ecs;
import inv;
import pog;
import rng;
import silog;

namespace mobs {
void attack_enemy(ecs::ec *ec, pog::eid src, pog::eid tgt) {
  const auto &src_mob = ec->mobs.get(src);
  const auto &tgt_mob = ec->mobs.get(tgt);

  int atk = rng::rand(src_mob.attack);
  int def = rng::rand(tgt_mob.defense);
  int margin = atk - def;

  silog::log(silog::debug, "%d attacks %d - %d v %d",
             static_cast<unsigned>(src), static_cast<unsigned>(tgt), atk, def);

  if (margin <= 0)
    return;

  if (tgt_mob.life > margin) {
    auto m = tgt_mob;
    m.life -= margin;
    ec->mobs.update(tgt, m);
    return;
  }

  if (ec->loot.has(tgt)) {
    auto lid = ec->loot.get(tgt)(ec);
    ec->coords.update(lid, ec->coords.get(tgt));
  }

  ecs::remove_mob(ec, tgt);
}

// returns true if position was changed
bool move_mob(ecs::ec *ec, pog::eid id, int dx, int dy) {
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

  if (ec->player.has(bid)) {
    attack_enemy(ec, id, bid);
    return false;
  }

  // TODO: allow enemies to attack each other
  if (ec->enemies.has(bid) && ec->player.has(id)) {
    attack_enemy(ec, id, bid);
    return false;
  }

  return false;
}

export void move_player(ecs::ec *ec, int dx, int dy) {
  auto pid = ec->player.get_id();
  if (move_mob(ec, pid, dx, dy))
    inv::take_items(ec);
}

export bool is_player_at_exit(ecs::ec *ec) {
  auto c0 = ec->coords.get(ec->player.get_id());
  auto c1 = ec->coords.get(ec->exit.get_id());
  return c0 == c1;
}

export void set_player_pos(ecs::ec *ec, unsigned x, unsigned y) {
  ec->coords.update(ec->player.get_id(), {x, y});
}

export void move_non_hostile_enemies(ecs::ec *ec) {
  const auto pc = ec->coords.get(ec->player.get_id());
  // TODO: change behaviour based on light level

  for (auto [_, id] : ec->non_hostiles) {
    switch (rng::rand(4)) {
    case 0:
      move_mob(ec, id, 0, -1);
      break;
    case 1:
      move_mob(ec, id, 0, 1);
      break;
    case 2:
      move_mob(ec, id, -1, 0);
      break;
    case 3:
      move_mob(ec, id, 1, 0);
      break;
    }
  }
}
} // namespace mobs
