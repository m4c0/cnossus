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
    if (m_life == 0)
      die();
  }

  void die() {}

protected:
  [[nodiscard]] virtual int attack_bonus() const noexcept = 0;
  [[nodiscard]] virtual int defense_bonus() const noexcept = 0;
  [[nodiscard]] virtual int damage_bonus() const noexcept = 0;

  virtual void run_actions_with_light(unsigned l) noexcept = 0;

  int damage_by(int margin) {
    if (margin > 0)
      m_damage_timer = 0.5;

    return m_life -= margin;
  }

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

  void process_actions_with_light(unsigned l) {
    if (m_actions <= 0) {
      m_actions += m_max_actions;
      process_poison();
      return;
    }
    m_actions -= m_type->dice();
    run_actions_with_light(l);
  }

  void update_animations(float dt) { m_damage_timer -= dt; }
};
} // namespace cno
