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
export int poison{};

export void init(int level) {
  if (level == 1) {
    life = initial_life;
    poison = 0;
  }
  coord = (level % 2) ? dotz::ivec2{1, 1} : dotz::ivec2{map::width - 2, 1};
}

export void draw() {
  if (life == 0)
    return;

  {
    constexpr const qsu::colour poisoned{0, 1, 0, 1};
    constexpr const qsu::colour normal{1, 1, 1, 1};
    qsu::guard::multiplier m{poison > 0 ? poisoned : normal};
    qsu::blit(spr::minotaur, coord.x, coord.y);
  }
  for (auto x = 0; x < life; x++) {
    qsu::blit(spr::minotaur, x, 0);
  }
}

export void hit(int roll, int poison) {
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
