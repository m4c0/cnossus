export module cno:player;

namespace cno {
/*
class player {
  mob m_mob{qsu::type{&minotaur}};
   inv::table m_inv{};

  void random_buff() {
    switch (rng::rand(3)) {
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
  [[nodiscard]] constexpr auto is_dead() const noexcept {
    return m_mob.life == 0;
  }

  void level_reset(unsigned lvl) {
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
};
*/
} // namespace cno
