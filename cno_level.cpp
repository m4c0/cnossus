module cnossus;
import enemies;
import inv;
import light;
import loot;
import player;
import save;

void cno::next_level() {
  auto level = ++save::d.level;
  map::gen(level);
  player::init(level);
  enemies::init(level);
  loot::init(level);
  light::init(level);
  inv::init(level);
  save::write();
}
