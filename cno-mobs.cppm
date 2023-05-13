export module cno:mobs;
import :entities;

namespace cno {
void tick_mobs(mob_list *mobs) {
  mobs->for_each([](auto &m) {
    if (m.life == 0)
      return;

    if (m.actions > 0) {
      m.actions -= m.type->dice;
      return;
    }

    if (m.poison > 0) {
      --m.poison;
      --m.life;
    }

    m.actions.to_max();
  });
}
} // namespace cno
