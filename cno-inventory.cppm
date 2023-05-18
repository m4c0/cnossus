export module cno:inventory;
import :itemtype;
import jute;
import qsu;

namespace cno::inv {
class slot {
  qsu::type<item_type> m_item;
  unsigned m_count{0};

public:
  constexpr slot() noexcept = default;
  explicit constexpr slot(qsu::type<item_type> i) noexcept : m_item{i} {}

  [[nodiscard]] constexpr auto count() const noexcept { return m_count; }

  [[nodiscard]] constexpr auto attack() const noexcept {
    return m_item->attack;
  }
  [[nodiscard]] constexpr auto defense() const noexcept {
    return m_item->defense;
  }

  [[nodiscard]] constexpr bool contains(qsu::type<item_type> i) const noexcept {
    return m_item->id == i->id;
  }

  [[nodiscard]] bool consume() noexcept {
    if (m_count == 0)
      return false;
    if (m_item->life_gain == 0 && m_item->light_provided == 0)
      return false;

    m_count--;
    return true;
  }

  [[nodiscard]] bool get_item(bool has_bag) noexcept {
    using namespace jute::literals;

    if (m_item->carry == carry_many) {
      if (m_count == 0 || has_bag) {
        m_count++;
        return true;
      }
      return false;
    }
    auto max = m_item->carry == carry_two ? 2 : 1;
    if (m_count < max) {
      m_count++;
      return true;
    }
    return false;
  }
};

class table {
  slot m_slots[item_type_count];

  [[nodiscard]] bool has_bag() const noexcept {
    for (auto &s : m_slots) {
      if (s.contains(qsu::type{&bag}))
        return s.count() != 0;
    }
    return false;
  }

public:
  constexpr table() {
    for (auto i = 0U; i < item_type_count; i++) {
      m_slots[i] = slot{qsu::type{item_types[i]}};
    }
  }

  [[nodiscard]] bool consume(qsu::type<item_type> it) noexcept {
    for (auto &s : m_slots) {
      if (s.contains(it))
        return s.consume();
    }
    return false;
  }

  [[nodiscard]] bool get_item(qsu::type<item_type> it) noexcept {
    const bool bag = has_bag();
    for (auto &s : m_slots) {
      if (s.contains(it))
        return s.get_item(bag);
    }
    return false;
  }

  void for_each(auto &&fn) const noexcept {
    for (auto &i : m_slots) {
      if (i.count() == 0)
        continue;
      fn(i);
    }
  }
};
} // namespace cno::inv
