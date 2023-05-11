export module cno:moblist;
import :mobtype;
import quack;

namespace cno {
struct bonus {
  int attack;
  int defense;
  int damage;
};
struct mob : sprite<mob_type> {
  static constexpr const auto initial_max_actions = 20;

  ranged life{type ? type->life : 0};
  ranged actions{initial_max_actions};
  unsigned poison{};
  float damage_timer{};
  bonus bonus{};
};
[[nodiscard]] constexpr auto is_player(const mob &a) noexcept {
  return a.type->hostility == h_none;
}

struct mob_list : public sbatch<mob, max_mobs_per_level> {
  using sbatch::sbatch;

  void fill_quack(map_coord pc, unsigned d) {
    sbatch::fill_quack(pc, d);

    const auto &[px, py] = pc;
    fill_colour([px, py, d](auto &i) {
      if (!i.type)
        return quack::colour{};

      const auto &[x, y] = i.coord;
      auto dx = px - x;
      auto dy = py - y;

      const auto &[r, b] = i.type->id.uv().start;
      auto a = (dx * dx + dy * dy) <= d ? 1.0f : 0.3f;
      return quack::colour{r, 0, b, a};
    });
  }
};
} // namespace cno
