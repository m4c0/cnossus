module cnossus;
import jute;
import save;
import sfx;
import spr;
import ui;

namespace {
  enum options {
    o_new_game,
    o_continue,
    o_none,
  };
}
static bool g_has_save {};

static void draw() {
  constexpr const auto x = 4.5;

  // MENU OPTIONS
  ui::draw_str("NEW GAME", x, -0.5);
  ui::draw_str("CONTINUE", x, 2.5, g_has_save ? 1.0 : 0.4);
}

static void redraw() { qsu::draw(draw); }

static auto mouse_sel() {
  auto [x, y] = qsu::mouse_pos();
  if (x < 4.5 || x > 12.5) return o_none;
  if (y > -0.5 && y < 0.5) return o_new_game;
  if (g_has_save && y > 2.5 && y < 3.5) return o_continue;
  return o_none; 
}

static void select() {
  switch (mouse_sel()) {
    case o_new_game:
      sfx::menu_select();
      // TODO: animate to "next level"
      save::reset();
      cno::next_level();
      cno::modes::game();
      break;
    case o_continue: 
      sfx::menu_select();
      cno::modes::game();
      break;
    case o_none: break;
  }
}

void cno::modes::mainmenu() {
  using namespace casein;

  save::read([] {
    g_has_save = true;
  }, [] {
    g_has_save = false;
  });

  cno::reset_casein();

  handle(GESTURE, G_TAP_1, select);
  handle(MOUSE_DOWN, select);

  redraw();

  constexpr const auto gs = 20;
  qsu::grid_size({
      .grid_pos = { -2 + gs / 2, 2 },
      .grid_size = { gs },
  });
}

void cno::load_options() {}
