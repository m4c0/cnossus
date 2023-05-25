export module inv;
import ecs;

namespace inv {
bool player_has_bag(ecs::ec *ec) {
  for (auto [_, id] : ec->in_use) {
    if (ec->bags.has(id))
      return true;
  }
  return false;
}
} // namespace inv
