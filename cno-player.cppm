export module cno:player;
import :inventory;
import :itemtype;
import :mobtype;
import :random;
import ecs;
import map;
import qsu;

namespace cno {
class player {
  mob m_mob{qsu::type{&minotaur}};
  inv::table m_inv{};

  void random_buff() {
    switch (cno::random(3)) {
    case 0:
      m_mob.bonus.damage++;
      break;
    case 1:
      m_mob.life.add_max(1);
      break;
    default:
      m_mob.actions.add_max(1);
      break;
    }
    m_mob.life += 1;
  }

  void update_inventory() {
    m_mob.bonus.defense = 0;
    m_inv.for_each([this](auto &i) {
      m_mob.bonus.defense += i.defense();
      if (i.attack() > m_mob.bonus.attack)
        m_mob.bonus.attack = i.attack();
    });
  }

public:
  [[nodiscard]] constexpr auto coord() const noexcept { return m_mob.coord; }
  [[nodiscard]] constexpr auto mob() noexcept { return &m_mob; }

  [[nodiscard]] constexpr auto is_dead() const noexcept {
    return m_mob.life == 0;
  }

  void add_entity(ecs::ec *ec) const noexcept {
    const auto &[x, y] = m_mob.coord;
    ecs::add_player(ec, {x, y});
  }

  void level_reset(unsigned lvl) {
    auto px = (lvl % 2 == 1) ? 1U : ::map::width - 2U;
    m_mob.coord = {px, 1};

    if (lvl > 1)
      random_buff();
  }

  [[nodiscard]] bool get_item(qsu::type<item_type> it) noexcept {
    if (!m_inv.get_item(it))
      return false;

    update_inventory();
    return true;
  }

  [[nodiscard]] bool consume(qsu::type<item_type> t) noexcept {
    if (!m_inv.consume(t))
      return false;

    m_mob.life += t->life_gain;
    return true;
  }

  constexpr void copy_mob_to(cno::mob &tgt) const noexcept { tgt = m_mob; }
};
} // namespace cno
