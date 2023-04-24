export module cno:inventory;
import :itemtype;
import jute;

namespace cno::inv {
template <typename Tp> class result {
  jute::twine<2> m_msg;
  Tp m_value;

public:
  constexpr result(jute::view a, Tp v) : m_msg{a + ""}, m_value{v} {}
  constexpr result(jute::view a, jute::view b, Tp v)
      : m_msg{a + b}, m_value{v} {}
};

class slot {
  const item_type *m_item;
  unsigned m_count{0};

  bool has_bag() {
    return false; // TODO
  }

public:
  constexpr slot() noexcept = default;
  explicit constexpr slot(const item_type *i) noexcept : m_item{i} {}

  constexpr void consume() noexcept { m_count--; }

  result<bool> get_item() noexcept {
    if (m_item->max_carry() == 0) {
      return {"That's hardly possible", "", false};
    }
    if (m_item->max_carry() == -1) {
      if (m_count == 0 || has_bag()) {
        m_count++;
        return {"You pick up a ", m_item->name(), true};
      } else {
        return {"You need a bag to carry more", false};
      }
    }
    if (m_count < m_item->max_carry()) {
      m_count++;
      return {"You pick up a ", m_item->name(), true};
    }
    return {"Already got enough of these", false};
  }
  result<const item_type *> drop_for_level(unsigned l) noexcept {
    return {"", nullptr};
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
};
} // namespace cno::inv
