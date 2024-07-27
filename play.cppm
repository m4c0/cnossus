export module play;
import dotz;
import enemies;
import inv;
import light;
import loot;
import map;
import party;
import player;
import qsu;
import quack;

namespace play {
extern dotz::vec2 pac;
static void draw() {
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

export void redraw(dotz::vec2 center = player::anim_coord()) {
  pac = center;
  quack::donald::data([](auto all) { return qsu::draw(all, draw); });
}
} // namespace play

module :private;
dotz::vec2 play::pac{};
