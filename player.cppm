export module player;
import dotz;
import map;
import rng;
import spr;
import qsu;

namespace player {
constexpr const auto base_attack = 10;
constexpr const auto base_defense = 10;
constexpr const auto base_life = 10;

export struct data {
  dotz::ivec2 coord{};
  int life{base_life};
  int max_life{base_life};
  int poison{};
  int attack{base_attack};
  int defense{base_defense};
} d;

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
}

constexpr const qsu::colour poisoned{0, 1, 0, 1};
constexpr const qsu::colour normal{1, 1, 1, 1};

export void draw() {
  if (d.life == 0)
    return;

  qsu::guard::multiplier m{d.poison > 0 ? poisoned : normal};
  qsu::blit(spr::minotaur, d.coord.x, d.coord.y);
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
