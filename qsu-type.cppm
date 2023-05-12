export module qsu:type;
import :idholder;

export namespace qsu {
template <id_holder Tp> class type {
  const Tp *m_type{};

public:
  constexpr type() noexcept = default;
  explicit constexpr type(const Tp *t) noexcept : m_type{t} {}

  [[nodiscard]] explicit constexpr operator bool() const noexcept {
    return m_type != nullptr;
  }
  [[nodiscard]] constexpr const Tp *operator->() const noexcept {
    return m_type;
  }
};
} // namespace qsu
