export module cno:player;
import :globals;
import :inventory;
import :itemtype;
import :mob;
import :mobtype;
import :random;

namespace cno {
class player : public mob {
  int m_armor{};
  int m_attack_bonus{};
  int m_damage_bonus{};
  unsigned m_extra_life{};
  map_coord m_next_step{};

protected:
  [[nodiscard]] int attack_bonus() const noexcept override {
    return m_attack_bonus;
  }
  [[nodiscard]] int defense_bonus() const noexcept override { return m_armor; }
  [[nodiscard]] int damage_bonus() const noexcept override {
    return m_damage_bonus;
  }

  [[nodiscard]] map_coord next_move_with_light(map_coord pp,
                                               unsigned l) noexcept override {
    return m_next_step;
  }

public:
  explicit player(map_coord c) : mob{&minotaur, c} {}

  [[nodiscard]] constexpr auto max_life() const noexcept {
    return minotaur.life() + m_extra_life;
  }

  void random_buff() {
    switch (cno::random(3)) {
    case 0:
      g::update_status("You feel stronger");
      m_damage_bonus++;
      break;
    case 1:
      g::update_status("You feel sturdier");
      m_extra_life++;
      break;
    default:
      g::update_status("You feel faster");
      increase_max_actions();
      break;
    }
    recover_health(1);
  }

  void recover_health(unsigned h) {
    auto r = max_life() - life();
    auto d = r > h ? h : r;
    damage_by(-d);
  }

  void set_next_move(int dx, int dy) {
    const auto &[x, y] = coord();
    m_next_step = {x + dx, y + dy};
  }

  void update_inventory(const inv::table &inv) {
    m_armor = 0;
    inv.for_each([this](auto &i) {
      m_armor += i.defense();
      if (i.attack() > m_attack_bonus)
        m_attack_bonus = i.attack();
    });
  }
};
} // namespace cno
