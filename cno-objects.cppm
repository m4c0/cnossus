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

class ranged {
  unsigned m_value;
  unsigned m_max;

public:
  constexpr explicit ranged(unsigned max) : m_value{max}, m_max{max} {}

  constexpr operator unsigned() const noexcept { return m_value; }

  constexpr auto &operator+=(unsigned delta) noexcept {
    auto n = m_value + delta;
    m_value = n > m_max ? m_max : n;
    return *this;
  }
  constexpr auto &operator-=(unsigned delta) noexcept {
    m_value = delta > m_value ? 0 : m_value - delta;
    return *this;
  }
  constexpr auto &operator--() noexcept { return *this -= 1; }

  constexpr void add_max(int delta) noexcept { m_max += delta; }
  constexpr void to_max() noexcept { m_value = m_max; }
};
} // namespace cno
