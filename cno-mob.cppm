export module cno:mob;
import :mobtype;
import :objects;

namespace cno {
class mob {
  static constexpr const auto max_actions = 20;

  const mob_type *m_type;
  map_coord m_coord;
  unsigned m_life;
  int m_actions;
  unsigned m_max_actions;
  unsigned m_poison{};

  [[nodiscard]] constexpr bool update_actions() noexcept {
    if (m_actions > 0) {
      m_actions -= m_type->dice();
      return false;
    }
    m_actions += m_max_actions;
    return true;
  }

  void process_poison() {
    if (m_poison == 0)
      return;

    m_poison--;
    m_life--;
    if (m_life == 0)
      die();
  }

  void die() {}

  void hunt_hero() {}
  void run_from_hero() {}
  void wander() {}

public:
  constexpr mob(const mob_type *t, map_coord c)
      : m_type{t}, m_coord{c}, m_life{m_type->life()}, m_actions{max_actions},
        m_max_actions{max_actions} {}

  void process_actions_with_light(unsigned l) {
    if (update_actions()) {
      process_poison();
      return;
    }
    if (m_type == &minotaur) {
      return;
    }

    switch (m_type->hostility()) {
    case h_none:
    case h_scaried:
      if (l < 1)
        wander();
      else
        run_from_hero();
      break;
    case h_aggresive:
      if (l < 1)
        hunt_hero();
      else
        wander();
      break;
    }
  }
};
} // namespace cno
