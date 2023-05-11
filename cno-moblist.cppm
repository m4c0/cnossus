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

using mob_list = sbatch<mob, max_mobs_per_level>;

void hide_mobs_in_fog(mob_list *m) {
  m->for_each([](auto &i) { i.vis = (i.vis == sv_fog) ? sv_none : i.vis; });
}
void debug_mobs(mob_list *m) {
  m->fill_colour([](auto &i) {
    auto a = i.vis == sv_none ? 0.0f : 1.0f;
    return quack::colour{1, 1, 1, a};
  });
}
} // namespace cno
