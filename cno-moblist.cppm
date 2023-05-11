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

  void update_rogueview(map_coord pc, unsigned d) {
    sbatch::update_rogueview(pc, d);

    sbatch::for_each([](auto &i) {
      if (i.vis == sv_fog)
        i.vis = sv_none;
    });
  }

  // TODO: remove this after add mob atlas
  void fill_quack() {
    sbatch::fill_quack();
    fill_colour([](auto &i) {
      auto a = i.vis == sv_none ? 0.0f : 1.0f;
      return quack::colour{1, 1, 1, a};
    });
  }
};
} // namespace cno
