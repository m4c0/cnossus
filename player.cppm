export module player;
import dotz;
import map;
import rng;
import save;
import spr;
import qsu;
import timeline;

namespace player {
// TODO: balance attack/defense
// TODO: hunger damage, to force food consumption
struct data {
  dotz::ivec2 coord{};
  dotz::vec2 anim_coord{};
  float rotation{};
} d;

export const auto coord() { return d.coord; }
export const bool is_dead() { return save::d.player.life <= 0; }
export const auto attack() { return save::d.player.attack; }
export const auto defense() { return save::d.player.defense; }

export void move(dotz::ivec2 c) {
  tim::add({
      .pos{
          .target = &d.anim_coord,
          .a = d.coord,
          .b = c,
      },
      .length = tim::anim_dur_ms,
  });
  d.anim_coord = c;
  d.coord = c;
}
export void attack(dotz::ivec2 c) {
  tim::add({
      .pos{
          .target = &d.anim_coord,
          .a = d.coord,
          .b = c,
          .func = tim::fn::half_and_back,
      },
      .length = tim::anim_dur_ms,
  });
}

export void init(int level) {
  if (level == 1) {
    d = {};
  } else {
    switch (rng::rand(4)) {
    case 0:
      save::d.player.max_life++;
      save::d.player.life++;
      break;
    case 1:
      save::d.player.attack++;
      break;
    case 2:
      save::d.player.defense++;
      break;
    default:
      break;
    }
  }
  d.coord = (level % 2) ? dotz::ivec2{1, 1} : dotz::ivec2{map::width - 2, 1};
  d.anim_coord = d.coord;
}

constexpr const qsu::colour poisoned{0, 1, 0, 1};
constexpr const qsu::colour normal{1, 1, 1, 1};

export auto anim_coord() { return d.anim_coord; }

export void draw() {
  if (save::d.player.life == 0)
    return;

  qsu::guard::multiplier m{save::d.player.poison > 0 ? poisoned : normal};
  qsu::blit(spr::minotaur, d.anim_coord, d.rotation);
}
export void draw_ui() {
  constexpr const auto x = -4.5;
  constexpr const auto y = 3.5;

  for (auto i = 0; i < save::d.player.life; i++) {
    qsu::guard::multiplier m{
        save::d.player.poison >= save::d.player.life - i ? poisoned : normal};
    qsu::blit(spr::minotaur, x, y - i * 8.5 / save::d.player.max_life, 0);
  }
}

export void poison_tick() {
  if (save::d.player.poison == 0 || save::d.player.life == 0)
    return;

  // TODO: animate
  if (rng::rand(2))
    save::d.player.life--;

  save::d.player.poison--;
}

export void restore(int roll) {
  // TODO: animate
  save::d.player.life += rng::rand(roll);
  if (save::d.player.life > save::d.player.max_life)
    save::d.player.life = save::d.player.max_life;
}

export void hit(int roll, int poison) {
  if (poison > 0)
    save::d.player.poison += rng::rand(poison);

  // TODO: animate
  if (roll <= 0)
    return;

  // TODO: animate
  auto dmg = rng::rand(roll);
  if (dmg == 0)
    return;

  tim::add({
      .rot{
          .target = &d.rotation,
          .a = 0,
          .b = 10,
          .func = tim::fn::half_and_back,
      },
      .start = tim::anim_dur_ms,
      .length = tim::anim_dur_ms,
  });

  save::d.player.life -= dmg;
  if (save::d.player.life < 0)
    save::d.player.life = 0;
}
} // namespace player
