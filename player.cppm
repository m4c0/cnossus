export module player;
import dotz;
import map;
import spr;
import quack;

namespace player {
export dotz::ivec2 coord{};

export void init(int level) {
  coord = (level % 2) ? dotz::ivec2{1, 1} : dotz::ivec2{map::width - 2, 1};
}

export auto draw(quack::mapped_buffers &all) {
  return spr::blit(spr::minotaur, coord.x, coord.y, all);
}
} // namespace player
