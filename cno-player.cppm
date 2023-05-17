export module cno:player;
import :globals;
import :inventory;
import :itemtype;
import :moblist;
import :random;
import map;
import qsu;

namespace cno {
class player {
  mob m_mob{qsu::type{&minotaur}};

protected:
  void random_buff() {
    switch (cno::random(3)) {
    case 0:
      g::update_status("You feel stronger");
      m_mob.bonus.damage++;
      break;
    case 1:
      g::update_status("You feel sturdier");
      m_mob.life.add_max(1);
      break;
    default:
      g::update_status("You feel faster");
      m_mob.actions.add_max(1);
      break;
    }
    m_mob.life += 1;
  }

public:
  [[nodiscard]] constexpr auto coord() const noexcept { return m_mob.coord; }
  [[nodiscard]] constexpr auto mob() noexcept { return &m_mob; }

  [[nodiscard]] constexpr auto is_dead() const noexcept {
    return m_mob.life == 0;
  }

  void consume(qsu::type<item_type> t) { m_mob.life += t->life_gain; }

  void update_inventory(const inv::table &inv) {
    m_mob.bonus.defense = 0;
    inv.for_each([this](auto &i) {
      m_mob.bonus.defense += i.defense();
      if (i.attack() > m_mob.bonus.attack)
        m_mob.bonus.attack = i.attack();
    });
  }

  void level_reset(unsigned lvl) {
    auto px = (lvl % 2 == 1) ? 1U : ::map::width - 2U;
    m_mob.coord = {px, 1};

    if (lvl > 1)
      random_buff();
  }

  constexpr void copy_mob_to(cno::mob &tgt) const noexcept { tgt = m_mob; }
};
} // namespace cno
