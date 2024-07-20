export module player;
import dotz;
import map;
import party;
import rng;
import spr;
import qsu;
import timeline;

namespace player {
// TODO: balance attack/defense
// TODO: hunger damage, to force food consumption
export struct data {
  static constexpr const auto base_attack = 10;
  static constexpr const auto base_defense = 10;
  static constexpr const auto base_life = 10;

  int life{base_life};
  int max_life{base_life};
  int poison{};
  int attack{base_attack};
  int defense{base_defense};

  // TODO: use qsu::spr
  dotz::ivec2 coord{};
  dotz::vec2 anim_coord{};
  float rotation{};
} d;

export const auto coord() { return d.coord; }
export const bool is_dead() { return d.life <= 0; }
export const auto attack() { return d.attack; }
export const auto defense() { return d.defense; }

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
      d.max_life++;
      d.life++;
      break;
    case 1:
      d.attack++;
      break;
    case 2:
      d.defense++;
      break;
    default:
      break;
    }
  }
  d.coord = (level % 2) ? dotz::ivec2{1, 1} : dotz::ivec2{map::width - 2, 1};
  d.anim_coord = d.coord;
}

constexpr const dotz::vec4 poisoned{0, 1, 0, 1};
constexpr const dotz::vec4 normal{1, 1, 1, 1};

export auto anim_coord() { return d.anim_coord; }

export void draw() {
  if (d.life == 0)
    return;

  qsu::guard::multiplier m{d.poison > 0 ? poisoned : normal};
  qsu::blit(spr::minotaur, d.anim_coord, d.rotation);
}
export void draw_ui() {
  constexpr const auto x = -4.5f;

  const auto &p = d;
  auto life = static_cast<float>(p.life) / p.max_life;

  auto colour = p.poison > 0 ? dotz::vec4{0, 1, 0, 1} : dotz::vec4{1, 0, 0, 1};

  qsu::draw_bar(life, spr::minotaur, x, colour);
}

export void poison_tick() {
  if (d.poison == 0 || d.life == 0)
    return;

  // TODO: animate
  if (rng::rand(2))
    d.life--;

  d.poison--;
}

export void restore(int roll) {
  // TODO: animate
  d.life += rng::rand(roll);
  if (d.life > d.max_life)
    d.life = d.max_life;
}

export void hit(int roll, int poison) {
  if (poison > 0)
    d.poison += rng::rand(poison);

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
          .b = rng::rand(2) ? -100.f : 100.f,
          .func = tim::fn::half_and_back,
      },
      .start = tim::anim_dur_ms,
      .length = tim::anim_dur_ms,
  });

  d.life -= dmg;
  if (d.life <= 0) {
    party::emit({
        .sprite{
            .id = spr::minotaur,
            .pos = d.coord,
        },
        .timeout = 1000,
    });
    d.life = 0;
  }
}
} // namespace player
