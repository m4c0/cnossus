export module cno:mobs;
import :entities;
import qsu;

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
  if (src.type->id == tgt.type->id)
    return {};

  const auto srcn = src.type->name;
  const auto tgtn = tgt.type->name;

  int atk_roll = roll_dice(src.type->dice, 2) + src.bonus.attack;
  int def_roll = roll_dice(tgt.type->dice, 2) + tgt.bonus.defense;
  int margin = atk_roll - def_roll;

  if (margin > 0) {
    margin += src.bonus.attack - tgt.bonus.defense;
    if (margin <= 0)
      return {};

    tgt.damage_timer = 0.5;
    tgt.life -= (tgt.life <= margin) ? tgt.life : margin;
    if (tgt.life == 0) {
      auto drop =
          qsu::sprite<item_type>{qsu::type{tgt.type->drops.roll()}, tgt.coord};
      tgt = {};
      return drop;
    } else if (src.type->poison > 0) {
      tgt.poison += 1 + cno::random(src.type->poison);
    }
  } else if (margin == 0) {
    if (src.type->poison > 0) {
      tgt.poison += 1 + cno::random(src.type->poison);
    }
  }
  return {};
}

void hide_mobs_in_fog(mob_list *m) {
  m->for_each(
      [](auto &i) { i.vis = (i.vis == qsu::v_fog) ? qsu::v_none : i.vis; });
}
} // namespace cno::mobs
