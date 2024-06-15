export module player;
import dotz;
import inv;
import map;
import rng;
import spr;
import quack;

static constexpr const auto initial_life = 20;

namespace player {
export dotz::ivec2 coord{};
export int life{};

export void init(int level) {
  if (level == 1) {
    life = initial_life;
  }
  coord = (level % 2) ? dotz::ivec2{1, 1} : dotz::ivec2{map::width - 2, 1};
}

export auto draw(quack::mapped_buffers &all) {
  if (life == 0)
    return 0U;

  auto count = spr::blit(spr::minotaur, coord.x, coord.y, all);
  for (auto x = 0; x < life; x++) {
    count += spr::blit(spr::minotaur, x, 0, all);
  }
  return count;
}

export void hit(int atk) {
  auto def = inv::defense();
  auto roll = atk - def;
  if (roll <= 0)
    return;

  life -= rng::rand(roll);
  if (life < 0)
    life = 0;
}
} // namespace player
