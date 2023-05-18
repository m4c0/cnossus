export module cno:light;
import :entities;
import :globals;
import qsu;

namespace cno {
class light {
  unsigned m_value{};

public:
  [[nodiscard]] constexpr bool too_bright_for_enemies() const noexcept {
    return m_value < 1;
  }
  [[nodiscard]] constexpr auto visible_distance() const noexcept {
    return m_value < 1 ? 2 : 5;
  }

  void consume(qsu::type<item_type> t) {
    if (t->light_provided > 0) {
      m_value += t->light_provided;
    }
  }

  void tick() {
    if (m_value == 0)
      return;

    m_value--;
  }
};
} // namespace cno
