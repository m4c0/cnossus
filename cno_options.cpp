module cnossus;
import dotz;
import sfx;
import sicfg;
import spr;

namespace {
enum options {
  o_sound,
  o_fullscreen,
  o_back,
};
}

static options g_sel{};

static void draw() {
  constexpr const auto x = 1.5;

  qsu::draw_str("OPTIONS", x, -2);

  if (sfx::main_volume > 0) {
    qsu::draw_str("SOUNDS", x, 0);
  } else {
    qsu::draw_str("MUTED", x, 0);
  }
  if (casein::fullscreen) {
    qsu::draw_str("FULLSCREEN", x, 1);
  } else {
    qsu::draw_str("WINDOWED", x, 1);
  }
  qsu::draw_str("BACK", x, 2);

  qsu::blit(spr::minotaur, 0, g_sel, 0);
}
static void redraw() { qsu::draw(draw); }

static void sel_up() {
  g_sel = static_cast<options>(g_sel - 1);
  if (g_sel < o_sound)
    g_sel = o_back;
  sfx::menu_move();
  redraw();
}
static void sel_down() {
  g_sel = static_cast<options>(g_sel + 1);
  if (g_sel > o_back)
    g_sel = o_sound;
  sfx::menu_move();
  redraw();
}

static void select() {
  sfx::menu_select();

  switch (g_sel) {
  case o_sound:
    sfx::main_volume = sfx::main_volume ? 0 : sfx::max_volume;
    redraw();
    break;
  case o_fullscreen:
    using namespace casein;
    fullscreen = !fullscreen;
    interrupt(IRQ_FULLSCREEN);
    redraw();
    break;
  case o_back:
#ifndef LECO_TARGET_WASM
    sicfg::boolean("muted", sfx::main_volume == 0);
    sicfg::boolean("fullscreen", fullscreen);
#endif
    cno::modes::mainmenu();
    break;
  }
}

void cno::modes::options() {
  using namespace casein;

  reset_casein();

  handle(KEY_DOWN, K_ESCAPE, modes::mainmenu);

  handle(KEY_DOWN, K_UP, sel_up);
  handle(KEY_DOWN, K_DOWN, sel_down);
  handle(KEY_DOWN, K_W, sel_up);
  handle(KEY_DOWN, K_S, sel_down);
  handle(KEY_DOWN, K_ENTER, select);

  redraw();
}

void cno::load_options() {
#ifndef LECO_TARGET_WASM
  sfx::main_volume = sicfg::boolean("muted") ? 0 : sfx::max_volume;
  casein::fullscreen = sicfg::boolean("fullscreen");
#endif
}
