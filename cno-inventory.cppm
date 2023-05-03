export module cno:inventory;
import :globals;
import :itemtype;
import jute;

namespace cno::inv {
class slot {
  const item_type *m_item;
  unsigned m_count{0};

public:
  constexpr slot() noexcept = default;
  explicit constexpr slot(const item_type *i) noexcept : m_item{i} {}

  [[nodiscard]] constexpr auto count() const noexcept { return m_count; }

  [[nodiscard]] constexpr auto attack() const noexcept {
    return m_item->attack();
  }
  [[nodiscard]] constexpr auto defense() const noexcept {
    return m_item->defense();
  }

  [[nodiscard]] constexpr bool contains(const item_type *i) const noexcept {
    return *m_item == *i;
  }

  [[nodiscard]] bool consume() noexcept {
    if (m_count == 0)
      return false;
    if (!m_item->consumable())
      return false;

    m_count--;
    return true;
  }

  [[nodiscard]] bool get_item(bool has_bag) noexcept {
    using namespace jute::literals;

    if (m_item->max_carry() == 0) {
      g::update_status("That's hardly possible");
      return false;
    }
    if (m_item->max_carry() == -1) {
      if (m_count == 0 || has_bag) {
        m_count++;
        g::update_status("You pick up a " + m_item->name());
        return true;
      } else {
        g::update_status("You need a bag to carry more");
        return false;
      }
    }
    if (m_count < m_item->max_carry()) {
      m_count++;
      g::update_status("You pick up a " + m_item->name());
      return true;
    }
    g::update_status("Already got enough of these");
    return false;
  }
};

class table {
  slot m_slots[item_type_count];

  [[nodiscard]] bool has_bag() const noexcept {
    for (auto &s : m_slots) {
      if (s.contains(&bag))
        return s.count() != 0;
    }
    return false;
  }

public:
  constexpr table() {
    for (auto i = 0U; i < item_type_count; i++) {
      m_slots[i] = slot{item_types[i]};
    }
  }

  [[nodiscard]] bool consume(const item_type *it) noexcept {
    for (auto &s : m_slots) {
      if (s.contains(it))
        return s.consume();
    }
    return false;
  }

  [[nodiscard]] bool get_item(const item_type *it) noexcept {
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
