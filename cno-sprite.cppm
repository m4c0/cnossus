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
concept sid_holder = requires(const Tp &x) {
  { x.id } -> traits::same_as<const sid &>;
};

[[nodiscard]] constexpr bool operator==(const sid_holder auto &a,
                                        const sid_holder auto &b) noexcept {
  return a.id == b.id;
}

template <sid_holder Tp> class sprite {
  const Tp *m_type{};

public:
  constexpr sprite() noexcept = default;
  explicit constexpr sprite(const Tp *t) noexcept : m_type{t} {}

  [[nodiscard]] explicit constexpr operator bool() const noexcept {
    return m_type != nullptr;
  }
  [[nodiscard]] constexpr const Tp *operator->() const noexcept {
    return m_type;
  }

  [[nodiscard]] constexpr bool operator==(const sprite<Tp> &o) const noexcept {
    return *m_type == *o.m_type;
  }
};
} // namespace cno
