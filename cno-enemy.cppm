export module cno:enemy;
import :objects;
import :mob;
import :mobtype;

namespace cno {
class enemy : public mob {
  int m_bonus;

  void hunt_hero() {}
  void run_from_hero() {}
  void wander() {}

protected:
  [[nodiscard]] int attack_bonus() const noexcept override { return m_bonus; }
  [[nodiscard]] int defense_bonus() const noexcept override { return m_bonus; }
  [[nodiscard]] int damage_bonus() const noexcept override { return 0; }

  void run_actions_with_light(unsigned l) noexcept override {
    switch (type()->hostility()) {
    case h_none:
    case h_scaried:
      if (l < 1)
        wander();
      else
        run_from_hero();
      break;
    case h_aggresive:
      if (l < 1)
        hunt_hero();
      else
        wander();
      break;
    }
  }

public:
  enemy(const mob_type *t, map_coord c, unsigned level)
      : mob{t, c}, m_bonus{static_cast<int>(level) / 2} {}
};
} // namespace cno
