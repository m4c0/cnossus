export module play;
import anim;
import dotz;
import enemies;
import inv;
import light;
import loot;
import map;
import party;
import player;
import silog;
import qsu;
import spr;

namespace play {
extern dotz::vec2 pac;
extern float fade;
static void draw() {
  qsu::grid_size({
      .grid_pos = {},
      .grid_size = {9, 9},
  });

  anim::fog_center = pac;
  anim::fog_radius = light::d.charge > 0 ? 2 : 1;
  {
    map::draw();
    loot::draw();
    enemies::draw();
    player::draw();
    party::draw();
  }

  player::draw_ui();
  light::draw_ui();
  inv::draw_ui();

  if (fade < 1.0) {
    qsu::blit(qsu::sprite{
        .id = spr::cursor,
        .pos{-10, -10},
        .size{20, 20},
        .colour{0.f, 0.f, 0.f, 1.f - fade},
        .alpha = 1,
        .rotation = 0,
    });
  }
}

export void redraw(dotz::vec2 center, float fade = 1.0) {
  pac = center;
  play::fade = fade;
  qsu::draw(draw);
}
export void redraw(float fade) { redraw(player::anim_coord(), fade); }
export void redraw() { redraw(1.0); }
} // namespace play

module :private;
dotz::vec2 play::pac{};
float play::fade{};
