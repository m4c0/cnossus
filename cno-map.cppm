export module cno:map;
import :blocktype;
import :globals;

namespace cno {
using block = sprite<block_type>;
using map = sbatch<block, map_width * map_height>;

[[nodiscard]] constexpr auto &map_at(map *m, unsigned x, unsigned y) noexcept {
  return m->at(y * map_width + x);
}
[[nodiscard]] constexpr auto &map_at(map *m, map_coord c) noexcept {
  return map_at(m, c.x, c.y);
}
} // namespace cno
