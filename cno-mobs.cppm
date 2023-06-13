export module cno:mobs;
import :entities;
import qsu;
import rng;

/*
namespace cno::mobs {
void tick(mob_list *mobs) {
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

void update_animations(mob_list *mobs, float dt) {
  mobs->for_each([dt](auto &m) { m.damage_timer -= dt; });
}

[[nodiscard]] qsu::sprite<item_type> attack(const mob &src, mob &tgt) {
  if (margin > 0) {
    tgt.damage_timer = 0.5;
      tgt.poison += 1 + rng::rand(src.type->poison);
  }
}
} // namespace cno::mobs
*/
