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
import spr;

namespace play {
extern dotz::vec2 pac;
extern float fade;
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

  if (fade < 1.0) {
    qsu::blit({
        .id = spr::cursor,
        .pos{-10, -10},
        .size{20, 20},
        .colour{0, 0, 0, 1},
        .alpha = fade,
        .rotation = 0,
    });
  }
}

export void redraw(dotz::vec2 center, float fade = 1.0) {
  pac = center;
  play::fade = fade;
  quack::donald::data([](auto all) { return qsu::draw(all, draw); });
}
export void redraw(float fade) { redraw(player::anim_coord(), fade); }
export void redraw() { redraw(1.0); }
} // namespace play

module :private;
dotz::vec2 play::pac{};
float play::fade{};
