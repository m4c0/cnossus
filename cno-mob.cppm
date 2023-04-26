export module cno:mob;
import :mobtype;
import :objects;

namespace cno {
class mob {
  static constexpr const auto max_actions = 20;

  const mob_type *m_type;
  map_coord m_coord;
  unsigned m_life;
  unsigned m_actions;
  unsigned m_max_actions;

public:
  constexpr mob(const mob_type *t, map_coord c)
      : m_type{t}, m_coord{c}, m_life{m_type->life()}, m_actions{max_actions},
        m_max_actions{max_actions} {}
};
} // namespace cno
