export module cno:enemy;
import :objects;
import :mob;
import :mobtype;

namespace cno {
class enemy : public mob {
  int m_bonus;

protected:
  [[nodiscard]] int attack_bonus() const noexcept { return m_bonus; }
  [[nodiscard]] int defense_bonus() const noexcept { return m_bonus; }
  [[nodiscard]] int damage_bonus() const noexcept { return 0; }

public:
  enemy(const mob_type *t, map_coord c, unsigned level)
      : mob{t, c}, m_bonus{static_cast<int>(level) / 2} {}
};
} // namespace cno
