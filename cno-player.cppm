export module cno:player;
import :globals;
import :inventory;
import :itemtype;
import :moblist;
import :random;

namespace cno {
class player {
  unsigned m_extra_life{};
  mob *m_mob;

protected:
  void random_buff() {
    switch (cno::random(3)) {
    case 0:
      g::update_status("You feel stronger");
      m_mob->bonus.damage++;
      break;
    case 1:
      g::update_status("You feel sturdier");
      m_extra_life++;
      break;
    default:
      g::update_status("You feel faster");
      m_mob->actions.add_max(1);
      break;
    }
    recover_health(1);
  }

public:
  explicit constexpr player(mob *m) : m_mob{m} { *m = {sprite{&minotaur}}; }

  [[nodiscard]] constexpr auto max_life() const noexcept {
    return minotaur.life + m_extra_life;
  }
  [[nodiscard]] constexpr auto coord() const noexcept { return m_mob->coord; }
  [[nodiscard]] constexpr auto mob() const noexcept { return m_mob; }

  [[nodiscard]] constexpr auto is_dead() const noexcept {
    return m_mob->life == 0;
  }

  void recover_health(unsigned h) {
    auto r = max_life() - m_mob->life;
    auto d = r > h ? h : r;
    m_mob->life += d;
  }

  void update_inventory(const inv::table &inv) {
    m_mob->bonus.defense = 0;
    inv.for_each([this](auto &i) {
      m_mob->bonus.defense += i.defense();
      if (i.attack() > m_mob->bonus.attack)
        m_mob->bonus.attack = i.attack();
    });
  }

  void level_reset(unsigned lvl) {
    auto px = (lvl % 2 == 1) ? 1U : map_width - 2U;
    m_mob->coord = {px, 1};

    if (lvl > 1)
      random_buff();
  }
};
} // namespace cno
