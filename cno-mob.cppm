export module cno:mob;
import :mobtype;
import :objects;
import :random;

namespace cno {
struct bonus {
  int attack;
  int defense;
  int damage;
};

class mob {
  static constexpr const auto max_actions = 20;

  const mob_type *m_type{};
  map_coord m_coord{};
  unsigned m_life{};
  int m_actions{};
  unsigned m_max_actions{};
  unsigned m_poison{};
  float m_damage_timer{};
  bonus m_bonus{};

  void process_poison() {
    if (m_poison == 0 || m_life == 0)
      return;

    m_poison--;
    m_life--;
  }

public:
  constexpr mob() noexcept = default;
  constexpr mob(const mob_type *t, map_coord c)
      : m_type{t}, m_coord{c}, m_life{m_type->life}, m_actions{max_actions},
        m_max_actions{max_actions} {}

  [[nodiscard]] constexpr operator bool() const noexcept {
    return m_type != nullptr;
  }

  [[nodiscard]] constexpr const auto &coord() const noexcept { return m_coord; }

  [[nodiscard]] constexpr auto bonus() noexcept { return m_bonus; }
  [[nodiscard]] constexpr const auto &bonus() const noexcept { return m_bonus; }

  [[nodiscard]] constexpr auto damaged() const noexcept {
    return m_damage_timer > 0;
  }
  [[nodiscard]] constexpr auto life() const noexcept { return m_life; }

  [[nodiscard]] constexpr auto character() const noexcept {
    return m_type->character;
  }
  [[nodiscard]] constexpr auto dice_roll(unsigned i) const noexcept {
    return roll_dice(m_type->dice, i);
  }
  [[nodiscard]] constexpr auto hostility() const noexcept {
    return m_type->hostility;
  }
  [[nodiscard]] constexpr auto name() const noexcept { return m_type->name; }
  [[nodiscard]] constexpr auto poison() const noexcept {
    return m_type->poison;
  }
  [[nodiscard]] constexpr auto random_drop() const noexcept {
    return m_type->drops.roll();
  }

  [[nodiscard]] constexpr bool same_species_as(const mob &o) const noexcept {
    return *o.m_type == *m_type;
  }

  void set_coord(map_coord c) noexcept { m_coord = c; }

  [[nodiscard]] constexpr auto is_player() const noexcept {
    return cno::is_player(m_type);
  }

  void increase_max_actions() { m_max_actions++; }

  void poison_by(unsigned p) noexcept { m_poison += p; }

  [[nodiscard]] bool update_actions() {
    if (m_actions <= 0) {
      m_actions += m_max_actions;
      process_poison();
      return false;
    }
    m_actions -= m_type->dice;
    return true;
  }

  void damage_by(int margin) {
    if (margin > 0)
      m_damage_timer = 0.5;

    m_life = (m_life <= margin) ? 0 : m_life - margin;
  }

  void update_animations(float dt) { m_damage_timer -= dt; }
};
} // namespace cno
