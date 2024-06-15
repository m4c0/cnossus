export module player;
import dotz;
import map;
import rng;
import spr;
import quack;

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

export auto draw(quack::mapped_buffers &all) {
  if (life == 0)
    return 0U;

  // TODO: show poison
  auto count = spr::blit(spr::minotaur, coord.x, coord.y, all);
  for (auto x = 0; x < life; x++) {
    count += spr::blit(spr::minotaur, x, 0, all);
  }
  return count;
}

export void hit(int roll, int poison) {
  if (roll <= 0)
    return;

  auto dmg = rng::rand(roll);
  if (dmg == 0)
    return;

  life -= dmg;
  if (life < 0)
    life = 0;

  player::poison += poison;
}
} // namespace player
