export module ecs;
export import :ec;
export import :entities;
import :random;
import pog;

namespace ecs {
[[nodiscard]] auto find_empty_location(ecs::ec *ec) noexcept {
  pog::grid_coord c;
  do {
    c.x = random(map_width);
    c.y = random(map_height);
  } while (ec->blockers.has(c));
  return c;
}
} // namespace ecs
