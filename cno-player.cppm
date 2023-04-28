export module cno:player;
import :globals;
import :inventory;
import :itemtype;
import :mob;
import :mobtype;
import :random;

namespace cno {
class player : public mob {
  const item_type *m_weapon;
  int m_armor{};
  int m_damage_bonus{};
  unsigned m_extra_life{};

protected:
  [[nodiscard]] int attack_bonus() const noexcept override {
    return m_weapon == nullptr ? 0 : m_weapon->attack();
  }
  [[nodiscard]] int defense_bonus() const noexcept override { return m_armor; }
  [[nodiscard]] int damage_bonus() const noexcept override {
    return m_damage_bonus;
  }

  [[nodiscard]] map_coord next_move_with_light(unsigned l) noexcept override {
    // TODO: store desired move
    return coord();
  }

public:
  explicit player(map_coord c) : mob{&minotaur, c} {}

  [[nodiscard]] constexpr auto max_life() const noexcept {
    return type()->life() + m_extra_life;
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

  void update_inventory(const inv::table &inv) {
    m_armor = 0;
    inv.for_each([this](auto &i) {
      auto it = i.type();
      m_armor += it->defense();
      if (it->attack() > attack_bonus())
        m_weapon = it;
    });
  }
};
} // namespace cno
