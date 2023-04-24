export module cno:inventory;
import :itemtype;

namespace cno::inv {
template <typename Tp> struct result {};

class slot {
  const item_type *m_item;
  unsigned m_count{0};

public:
  constexpr slot() noexcept = default;
  explicit constexpr slot(const item_type *i) noexcept : m_item{i} {}

  constexpr void consume() noexcept { m_count--; }

  result<bool> get_item() noexcept { return {}; }
  result<const item_type *> drop_for_level(unsigned l) noexcept { return {}; }
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
