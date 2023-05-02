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
  float m_damage_timer{};

  void process_poison() {
    if (m_poison == 0)
      return;

    m_poison--;
    m_life--;
  }

protected:
  [[nodiscard]] virtual map_coord next_move_with_light(map_coord player_pos,
                                                       unsigned l) noexcept = 0;

  void increase_max_actions() { m_max_actions++; }

public:
  constexpr mob(const mob_type *t, map_coord c)
      : m_type{t}, m_coord{c}, m_life{m_type->life()}, m_actions{max_actions},
        m_max_actions{max_actions} {}
  constexpr virtual ~mob() = default;

  [[nodiscard]] constexpr auto &coord() noexcept { return m_coord; }
  [[nodiscard]] constexpr auto coord() const noexcept { return m_coord; }
  [[nodiscard]] constexpr auto damaged() const noexcept {
    return m_damage_timer > 0;
  }
  [[nodiscard]] constexpr auto life() const noexcept { return m_life; }
  [[nodiscard]] constexpr auto type() const noexcept { return m_type; }

  [[nodiscard]] virtual int attack_bonus() const noexcept = 0;
  [[nodiscard]] virtual int defense_bonus() const noexcept = 0;
  [[nodiscard]] virtual int damage_bonus() const noexcept = 0;

  [[nodiscard]] constexpr auto is_player() const noexcept {
    return m_type->is_player();
  }

  void poison_by(unsigned p) noexcept { m_poison += p; }

  [[nodiscard]] map_coord act_with_light(map_coord player_pos, unsigned l) {
    if (m_actions <= 0) {
      m_actions += m_max_actions;
      process_poison();
      return m_coord;
    }
    m_actions -= m_type->dice();
    return next_move_with_light(player_pos, l);
  }

  int damage_by(int margin) {
    if (margin > 0)
      m_damage_timer = 0.5;

    return m_life -= margin;
  }

  void update_animations(float dt) { m_damage_timer -= dt; }
};
} // namespace cno
