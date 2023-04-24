export module cno:inventory;
import :itemtype;

namespace cno {
struct inventory_slot {
  const item_type *item;
  unsigned count{};
};
class inventory {
  inventory_slot m_slots[item_type_count];

public:
  constexpr inventory() {
    for (auto i = 0U; i < item_type_count; i++) {
      m_slots[i] = {item_types[i], 0};
    }
  }
};
} // namespace cno
