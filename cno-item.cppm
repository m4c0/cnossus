export module cno:item;
import :itemtype;
import :objects;

namespace cno {
class item {
  const item_type *m_type;
  map_coord m_coord;

public:
  constexpr item() noexcept = default;
  constexpr item(const item_type *t, map_coord c) : m_type{t}, m_coord{c} {}

  [[nodiscard]] constexpr auto coord() const noexcept { return m_coord; }
  [[nodiscard]] constexpr auto type() const noexcept { return m_type; }

  [[nodiscard]] item open_at_level(unsigned l) const noexcept {
    auto nt = m_type->drop_for_level(l);
    return item{nt, m_coord};
  }
};
} // namespace cno
