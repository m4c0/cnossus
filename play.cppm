export module play;
import enemies;
import inv;
import light;
import loot;
import map;
import party;
import player;
import qsu;
import quack;

// TODO: fix phantom loot when enemies (and chests) die
namespace play {
export void draw() {
  auto pac = player::anim_coord();
  auto radius = light::d.charge > 0 ? 2 : 1;
  {
    qsu::guard::position ppo{-(pac + 0.5f)};
    map::draw(pac, radius);
    loot::draw(pac, radius);
    enemies::draw(pac, radius);
    player::draw();
    party::draw();
  }

  player::draw_ui();
  light::draw_ui();
  inv::draw_ui();
}
export void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, draw); });
}
} // namespace play
