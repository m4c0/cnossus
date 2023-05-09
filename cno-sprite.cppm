export module cno:sprite;
import quack;
import traits;

namespace cno {
class sid {
  char m_value;

public:
  constexpr sid(char v) : m_value{v} {}

  [[nodiscard]] constexpr operator char() const noexcept { return m_value; }

  [[nodiscard]] constexpr bool operator==(const sid &o) const noexcept {
    return m_value == o.m_value;
  }

  [[nodiscard]] constexpr quack::uv uv() const noexcept {
    auto u = static_cast<float>(m_value % 16);
    auto v = static_cast<float>(m_value / 16);

    auto u0 = u / 16.0f;
    auto v0 = v / 16.0f;
    auto u1 = (u + 1.0f) / 16.0f;
    auto v1 = (v + 1.0f) / 16.0f;
    return quack::uv{{u0, v0}, {u1, v1}};
  }
};

template <typename Tp>
concept sprite_holder = requires(const Tp &x) {
  { x.id } -> traits::same_as<const sid &>;
};

[[nodiscard]] constexpr bool operator==(const sprite_holder auto &a,
                                        const sprite_holder auto &b) noexcept {
  return a.id == b.id;
}
} // namespace cno
