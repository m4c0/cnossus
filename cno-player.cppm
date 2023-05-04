export module cno:player;
import :globals;
import :inventory;
import :itemtype;
import :mob;
import :mobtype;
import :random;

namespace cno {
class player {
  int m_armor{};
  int m_attack_bonus{};
  int m_damage_bonus{};
  unsigned m_extra_life{};
  mob *m_mob;

protected:
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
      m_mob->increase_max_actions();
      break;
    }
    recover_health(1);
  }

public:
  explicit constexpr player(mob *m) : m_mob{m} {}

  [[nodiscard]] constexpr auto max_life() const noexcept {
    return minotaur.life() + m_extra_life;
  }
  [[nodiscard]] constexpr auto coord() const noexcept { return m_mob->coord(); }
  [[nodiscard]] constexpr auto mob() const noexcept { return m_mob; }

  [[nodiscard]] constexpr auto is_dead() const noexcept {
    return m_mob->life() == 0;
  }

  void recover_health(unsigned h) {
    auto r = max_life() - m_mob->life();
    auto d = r > h ? h : r;
    m_mob->damage_by(-d);
  }

  void update_inventory(const inv::table &inv) {
    m_armor = 0;
    inv.for_each([this](auto &i) {
      m_armor += i.defense();
      if (i.attack() > m_attack_bonus)
        m_attack_bonus = i.attack();
    });
  }

  void level_reset(unsigned lvl) {
    auto px = (lvl % 2 == 1) ? 1U : map_width - 2U;
    m_mob->set_coord({px, 1});

    if (lvl > 1)
      random_buff();
  }
};
} // namespace cno
