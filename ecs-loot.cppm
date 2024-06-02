export module ecs:loot;
import :ec;
import rng;

namespace ecs {
template <unsigned MaxElements> class loot_table {
  rng::random_picker m_picker{MaxElements, 1};
  c::loot m_elements[MaxElements]{};

public:
  explicit loot_table(auto... is) : m_elements{is...} {}

  [[nodiscard]] constexpr auto operator[](unsigned elem) noexcept {
    return m_picker[elem];
  }

  [[nodiscard]] c::loot pick() const noexcept {
    return m_elements[m_picker.pick()];
  }
};
} // namespace ecs
