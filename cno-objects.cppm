export module cno:objects;

namespace cno {
struct map_coord {
  unsigned x;
  unsigned y;
};
[[nodiscard]] constexpr bool operator==(const map_coord &a,
                                        const map_coord &b) noexcept {
  return a.x == b.x && a.y == b.y;
}
} // namespace cno
