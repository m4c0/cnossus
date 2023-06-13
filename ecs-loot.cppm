export module ecs:loot;
import rng;

namespace ecs {
template <typename Tp, unsigned MaxElements> class loot_table {
  rng::random_picker<MaxElements> m_picker{1};
  Tp m_elements[MaxElements]{};

public:
  explicit loot_table(auto... is) : m_elements{is...} {}

  [[nodiscard]] constexpr auto operator[](unsigned elem) noexcept {
    return m_picker[elem];
  }

  [[nodiscard]] Tp pick() const noexcept { return m_elements[m_picker.pick()]; }
};
template <typename Tp, typename... T>
loot_table(Tp, T...) -> loot_table<Tp, sizeof...(T) + 1>;
} // namespace ecs
