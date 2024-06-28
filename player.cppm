export module player;
import dotz;
import map;
import rng;
import spr;
import qsu;
import timeline;

namespace player {
constexpr const auto base_attack = 10;
constexpr const auto base_defense = 10;
constexpr const auto base_life = 10;

// TODO: balance attack/defense
// TODO: hunger damage, to force food consumption
struct data {
  dotz::ivec2 coord{};
  dotz::vec2 anim_coord{};
  int life{base_life};
  int max_life{base_life};
  int poison{};
  int attack{base_attack};
  int defense{base_defense};
} d;

export const auto coord() { return d.coord; }
export const bool is_dead() { return d.life <= 0; }
export const auto attack() { return d.attack; }
export const auto defense() { return d.defense; }

export void move(dotz::ivec2 c) {
  tim::add({
      .target = &d.anim_coord,
      .a = d.coord,
      .b = c,
      .length = tim::anim_dur_ms,
  });
  d.anim_coord = c;
  d.coord = c;
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

constexpr const qsu::colour poisoned{0, 1, 0, 1};
constexpr const qsu::colour normal{1, 1, 1, 1};

export auto anim_coord() { return d.anim_coord; }

export void draw() {
  if (d.life == 0)
    return;

  qsu::guard::multiplier m{d.poison > 0 ? poisoned : normal};
  qsu::blit(spr::minotaur, d.anim_coord);
}
export void draw_ui() {
  constexpr const auto x = -4.5;
  constexpr const auto y = 3.5;

  for (auto i = 0; i < d.life; i++) {
    qsu::guard::multiplier m{d.poison >= d.life - i ? poisoned : normal};
    qsu::blit(spr::minotaur, x, y - i * 8.5 / d.max_life);
  }
}

export void poison_tick() {
  if (d.poison == 0 || d.life == 0)
    return;

  if (rng::rand(2))
    d.life--;

  d.poison--;
}

export void restore(int roll) {
  d.life += rng::rand(roll);
  if (d.life > d.max_life)
    d.life = d.max_life;
}

export void hit(int roll, int poison) {
  if (poison > 0)
    d.poison += rng::rand(poison);

  if (roll <= 0)
    return;

  auto dmg = rng::rand(roll);
  if (dmg == 0)
    return;

  d.life -= dmg;
  if (d.life < 0)
    d.life = 0;
}
} // namespace player
