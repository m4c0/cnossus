export module qsu:id;
import quack;

export namespace qsu {
class id {
  char m_value;

public:
  constexpr id(char v) : m_value{v} {}

  [[nodiscard]] constexpr operator char() const noexcept { return m_value; }

  [[nodiscard]] constexpr bool operator==(const id &o) const noexcept {
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
} // namespace qsu
