export module player;
import dotz;
import map;
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
  return spr::blit(spr::minotaur, coord.x, coord.y, all);
}

export void hit(int roll) {
  // TODO: defense roll

  life--;
  if (life < 0)
    life = 0;
}
} // namespace player
