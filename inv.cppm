export module inv;
export import :domain;
import ecs;
import pog;

namespace inv {
bool player_has_bag(ecs::ec *ec) {
  for (auto [_, id] : ec->in_use) {
    if (ec->bags.has(id))
      return true;
  }
  return false;
}

export void take_items(ecs::ec *ec) {
  auto pc = ec->coords.get(ec->player.get_id());
  ec->usables.remove_if([&](auto _, auto id) -> bool {
    if (pc != ec->coords.get(id))
      return false;

    ecs::remove_item(ec, id);
    return false;
  });
}

} // namespace inv
