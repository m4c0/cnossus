export module ecs:loot;
import rng;

namespace ecs {
template <typename Tp, unsigned MaxElements> class loot_table {
  rng::random_picker<MaxElements> m_picker{};
  Tp m_elements[MaxElements]{};

public:
  explicit loot_table(auto... is) : m_elements{is...} {}

  [[nodiscard]] constexpr auto operator[](unsigned elem) noexcept {
    return m_picker[elem];
  }

  [[nodiscard]] Tp pick() const noexcept { return m_elements[m_picker.pick()]; }
  void pick(auto fn) const noexcept {
    Tp p = pick();
    if (p != nullptr)
      fn(p);
  }
};
} // namespace ecs
