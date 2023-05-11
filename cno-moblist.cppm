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

    fill_colour([](auto &i) {
      if (i.vis == sv_visible)
        return quack::colour{1, 0, 1, 1};
      else
        return quack::colour{};
    });
  }
};
} // namespace cno
