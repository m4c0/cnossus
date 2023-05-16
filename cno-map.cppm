export module cno:map;
import :entities;
import :globals;
import map;

namespace cno {
[[nodiscard]] constexpr auto &map_at(map *m, unsigned x, unsigned y) noexcept {
  return m->at(y * ::map::width + x);
}
[[nodiscard]] constexpr auto &map_at(map *m, map_coord c) noexcept {
  return map_at(m, c.x, c.y);
}
} // namespace cno
