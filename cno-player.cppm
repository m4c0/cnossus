export module cno:player;
import :mob;
import :mobtype;

namespace cno {
class player : public mob {
protected:
  [[nodiscard]] int attack_bonus() const noexcept override { return 0; }
  [[nodiscard]] int defense_bonus() const noexcept override { return 0; }
  [[nodiscard]] int damage_bonus() const noexcept override { return 0; }

public:
  explicit player(map_coord c) : mob{&minotaur, c} {}
};
} // namespace cno
