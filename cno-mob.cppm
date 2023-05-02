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
  void increase_max_actions() { m_max_actions++; }

public:
  constexpr mob(const mob_type *t, map_coord c)
      : m_type{t}, m_coord{c}, m_life{m_type->life()}, m_actions{max_actions},
        m_max_actions{max_actions} {}
  constexpr virtual ~mob() = default;

  [[nodiscard]] constexpr const auto &coord() const noexcept { return m_coord; }
  [[nodiscard]] constexpr auto damaged() const noexcept {
    return m_damage_timer > 0;
  }
  [[nodiscard]] constexpr auto life() const noexcept { return m_life; }

  [[nodiscard]] constexpr auto character() const noexcept {
    return m_type->character();
  }
  [[nodiscard]] constexpr auto dice_roll(unsigned i) const noexcept {
    return m_type->dice_roll(i);
  }
  [[nodiscard]] constexpr auto hostility() const noexcept {
    return m_type->hostility();
  }
  [[nodiscard]] constexpr auto name() const noexcept { return m_type->name(); }
  [[nodiscard]] constexpr auto poison() const noexcept {
    return m_type->poison();
  }
  [[nodiscard]] constexpr auto random_drop() const noexcept {
    return m_type->random_drop();
  }

  [[nodiscard]] constexpr bool same_species_as(const mob &o) const noexcept {
    return *o.m_type == *m_type;
  }

  void set_coord(map_coord c) noexcept { m_coord = c; }

  [[nodiscard]] virtual int attack_bonus() const noexcept = 0;
  [[nodiscard]] virtual int defense_bonus() const noexcept = 0;
  [[nodiscard]] virtual int damage_bonus() const noexcept = 0;

  [[nodiscard]] constexpr auto is_player() const noexcept {
    return m_type->is_player();
  }

  [[nodiscard]] virtual map_coord next_move_with_light(map_coord player_pos,
                                                       unsigned l) noexcept = 0;

  void poison_by(unsigned p) noexcept { m_poison += p; }

  [[nodiscard]] bool update_actions() {
    if (m_actions <= 0) {
      m_actions += m_max_actions;
      process_poison();
      return false;
    }
    m_actions -= m_type->dice();
    return true;
  }

  int damage_by(int margin) {
    if (margin > 0)
      m_damage_timer = 0.5;

    return m_life -= margin;
  }

  void update_animations(float dt) { m_damage_timer -= dt; }
};
} // namespace cno
