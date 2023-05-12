export module cno:moblist;
import :entities;
import :mobtype;
import qsu;
import quack;

namespace cno {
[[nodiscard]] constexpr auto is_player(const mob &a) noexcept {
  return a.type->hostility == h_none;
}

void hide_mobs_in_fog(mob_list *m) {
  m->for_each(
      [](auto &i) { i.vis = (i.vis == qsu::v_fog) ? qsu::v_none : i.vis; });
}
void debug_mobs(mob_list *m) {
  m->fill_colour([](auto &i) {
    auto a = i.vis == qsu::v_none ? 0.0f : 1.0f;
    return quack::colour{1, 1, 1, a};
  });
}
} // namespace cno
