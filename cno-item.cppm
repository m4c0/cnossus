export module cno:item;
import :itemtype;

namespace cno {
struct map_coord {
  unsigned x;
  unsigned y;
};
class item {
  const item_type *m_type;
  map_coord m_coord;

public:
  constexpr item(const item_type *t, map_coord c) : m_type{t}, m_coord{c} {}

  [[nodiscard]] result<item> open_at_level(unsigned l) const noexcept {
    auto nt = m_type->drop_for_level(l);
    return {nt.message(), item{nt.value(), m_coord}};
  }
};
} // namespace cno
