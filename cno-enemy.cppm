export module cno:enemy;
import :objects;
import :mob;
import :mobtype;

namespace cno {
class enemy : public mob {
  int m_bonus;

  map_coord hunt_hero() { return coord(); }
  map_coord run_from_hero() { return coord(); }
  map_coord wander() { return coord(); }

protected:
  [[nodiscard]] int attack_bonus() const noexcept override { return m_bonus; }
  [[nodiscard]] int defense_bonus() const noexcept override { return m_bonus; }
  [[nodiscard]] int damage_bonus() const noexcept override { return 0; }

  [[nodiscard]] map_coord next_move_with_light(unsigned l) noexcept override {
    switch (type()->hostility()) {
    case h_none:
    case h_scaried:
      if (l < 1)
        return wander();
      else
        return run_from_hero();
    case h_aggresive:
      if (l < 1)
        return hunt_hero();
      else
        return wander();
    }
  }

public:
  enemy(const mob_type *t, map_coord c, unsigned level)
      : mob{t, c}, m_bonus{static_cast<int>(level) / 2} {}
};
} // namespace cno
