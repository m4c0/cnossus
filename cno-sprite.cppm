export module cno:sprite;
import traits;

namespace cno {
struct scoord {
  float u;
  float v;
};

class sid {
  char m_value;

public:
  constexpr sid(char v) : m_value{v} {}

  [[nodiscard]] constexpr bool operator==(const sid &o) const noexcept {
    return m_value == o.m_value;
  }
  [[nodiscard]] constexpr scoord uv() const noexcept {
    auto x = static_cast<float>(m_value % 16) / 16.0f;
    auto y = static_cast<float>(m_value / 16) / 16.0f;
    return {x, y};
  }
};

template <typename Tp>
concept sprite_holder = requires {
  { Tp::id } -> traits::same_as<sid>;
};

[[nodiscard]] constexpr bool operator==(const sprite_holder auto &a,
                                        const sprite_holder auto &b) noexcept {
  return a.id == b.id;
}
} // namespace cno
