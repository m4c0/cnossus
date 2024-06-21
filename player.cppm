export module player;
import dotz;
import map;
import rng;
import spr;
import qsu;

static constexpr const auto initial_life = 20;

namespace player {
export dotz::ivec2 coord{};
export int life{};
export int poison{5};

export void init(int level) {
  if (level == 1) {
    life = initial_life;
    poison = 0;
  } else {
    // TODO: give random buff: max life, damage, etc
  }
  coord = (level % 2) ? dotz::ivec2{1, 1} : dotz::ivec2{map::width - 2, 1};
}

constexpr const qsu::colour poisoned{0, 1, 0, 1};
constexpr const qsu::colour normal{1, 1, 1, 1};

export void draw() {
  if (life == 0)
    return;

  qsu::guard::multiplier m{poison > 0 ? poisoned : normal};
  qsu::blit(spr::minotaur, coord.x, coord.y);
}
export void draw_ui() {
  constexpr const auto x = -4.5;
  constexpr const auto y = 3.5;

  for (auto i = 0; i < life; i++) {
    qsu::guard::multiplier m{poison >= life - i ? poisoned : normal};
    qsu::blit(spr::minotaur, x, y - i * 8.5 / initial_life);
  }
}

export void poison_tick() {
  if (poison == 0 || life == 0)
    return;

  if (rng::rand(2))
    life--;

  poison--;
}

export void restore(int roll) {
  life += rng::rand(roll);
  if (life > initial_life)
    life = initial_life;
}

export void hit(int roll, int poison) {
  if (poison > 0)
    player::poison += rng::rand(poison);

  if (roll <= 0)
    return;

  auto dmg = rng::rand(roll);
  if (dmg == 0)
    return;

  life -= dmg;
  if (life < 0)
    life = 0;
}
} // namespace player
