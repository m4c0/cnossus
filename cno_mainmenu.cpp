module cnossus;
import jute;
import save;
import sfx;
import spr;
import quack;

namespace {
enum options {
  o_new_game,
  o_continue,
  o_options,
  o_exit,
};
}
static options g_sel{};
static bool g_has_save{};

static void draw() {
  constexpr const auto x = 1.5;

  // TODO: splash
  // TODO: credits
  // TODO: random play area on the background
  // SPLASH

  // MENU OPTIONS
  qsu::draw_str("NEW GAME", x, 0);
  qsu::draw_str("CONTINUE", x, 1, g_has_save ? 1.0 : 0.4);
  qsu::draw_str("OPTIONS", x, 2);
  qsu::draw_str("EXIT", x, 3);

  // MENU SELECTION
  qsu::blit(spr::minotaur, 0, g_sel, 0);
}

static void redraw() { qsu::draw(draw); }

static void sel_up() {
  g_sel = static_cast<options>(g_sel - 1);
  if (g_sel < o_new_game)
    g_sel = o_exit;
  if (!g_has_save && g_sel == o_continue)
    g_sel = o_new_game;
  sfx::menu_move();
  redraw();
}
static void sel_down() {
  g_sel = static_cast<options>(g_sel + 1);
  if (g_sel > o_exit)
    g_sel = o_new_game;
  if (!g_has_save && g_sel == o_continue)
    g_sel = o_options;
  sfx::menu_move();
  redraw();
}

static void select() {
  sfx::menu_select();

  switch (g_sel) {
  case o_new_game:
    // TODO: animate to "next level"
    save::reset();
    cno::next_level();
    cno::modes::game();
    break;
  case o_continue:
    cno::modes::game();
    break;
  case o_options:
    cno::modes::options();
    break;
  case o_exit:
    casein::interrupt(casein::IRQ_QUIT);
    break;
  }
}

void cno::modes::mainmenu() {
  using namespace casein;

  // TODO: lock menu until we know the state of the game
  save::read([] {
    // Let's avoid players from losing their saves by mistake
    if (g_sel == o_new_game)
      g_sel = o_continue;

    g_has_save = true;
  }, [] {
    // Consider no save if reading fail. Might be the case if save data is
    // corrupted after game started (also the way "game over" works :)
    if (g_sel == o_continue)
      g_sel = o_new_game;

    g_has_save = false;
  });

  cno::reset_casein();

  handle(KEY_DOWN, K_UP, sel_up);
  handle(KEY_DOWN, K_DOWN, sel_down);
  handle(KEY_DOWN, K_W, sel_up);
  handle(KEY_DOWN, K_S, sel_down);
  handle(KEY_DOWN, K_ENTER, select);

  redraw();

  constexpr const auto gs = 20;
  qsu::grid_size({
      .grid_pos = {-2 + gs / 2, 2},
      .grid_size = {gs},
  });
}
