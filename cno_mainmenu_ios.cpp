module cnossus;
import jute;
import save;
import sfx;
import spr;

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
  qsu::draw_str("NEW GAME", x, -0.5);
  qsu::draw_str("CONTINUE", x, 2.5, g_has_save ? 1.0 : 0.4);
}

static void redraw() {
  quack::donald::data([](auto & all) { return qsu::draw(all, ::draw); });
}

static auto mouse_sel() { return o_none; }

static void select() {
  sfx::menu_select();

  switch (mouse_sel()) {
    case o_new_game:
      // TODO: animate to "next level"
      save::reset();
      cno::next_level();
      cno::modes::game();
      break;
    case o_continue: cno::modes::game(); break;
    case o_none: break;
  }
}

void cno::modes::mainmenu() {
  using namespace casein;

  save::read().map([] { g_has_save = true; }).log_error([] { g_has_save = false; });

  cno::reset_casein();

  handle(GESTURE, G_TAP_1, select);

  redraw();

  constexpr const auto gs = 20;
  quack::donald::push_constants({
      .grid_pos = { -2 + gs / 2, 2 },
      .grid_size = { gs },
  });
}

void cno::load_options() {}
