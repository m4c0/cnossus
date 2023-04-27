export module cno:inventory;
import :globals;
import :itemtype;
import jute;

namespace cno::inv {
class slot {
  const item_type *m_item;
  unsigned m_count{0};

  bool has_bag() {
    return false; // TODO
  }

public:
  constexpr slot() noexcept = default;
  explicit constexpr slot(const item_type *i) noexcept : m_item{i} {}

  [[nodiscard]] constexpr auto count() const noexcept { return m_count; }
  [[nodiscard]] constexpr const auto *type() const noexcept { return m_item; }

  constexpr void consume() noexcept { m_count--; }

  [[nodiscard]] bool get_item() noexcept {
    using namespace jute::literals;

    if (m_item->max_carry() == 0) {
      g::update_status("That's hardly possible");
      return false;
    }
    if (m_item->max_carry() == -1) {
      if (m_count == 0 || has_bag()) {
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

public:
  constexpr table() {
    for (auto i = 0U; i < item_type_count; i++) {
      m_slots[i] = slot{item_types[i]};
    }
  }

  [[nodiscard]] bool get_item(const item_type *it) noexcept {
    for (auto &s : m_slots) {
      if (s.type() != it)
        continue;

      return s.get_item();
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
