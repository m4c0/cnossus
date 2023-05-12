export module qsu:coord;

export namespace qsu {
struct coord {
  unsigned x;
  unsigned y;
};
[[nodiscard]] constexpr bool operator==(const coord &a,
                                        const coord &b) noexcept {
  return a.x == b.x && a.y == b.y;
}
} // namespace qsu
