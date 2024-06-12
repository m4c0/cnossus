export module play;
import enemies;
import loot;
import map;
import player;
import quack;

namespace play {
export void init(int level) {
  map::gen(level);
  player::init(level);
  enemies::init(level);
  loot::init(level);
}

export auto draw(quack::mapped_buffers &all) {
  auto count = map::draw(all);
  count += loot::draw(all);
  count += enemies::draw(all);
  count += player::draw(all);
  return count;
}
} // namespace play
