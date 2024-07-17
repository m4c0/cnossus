module cnossus;
import jute;
import save;
import spr;

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

static void draw_str(jute::view str, float x, float y, float a = 1.0) {
  for (unsigned c : str) {
    qsu::blit(qsu::sprite{
        .id = static_cast<spr::id>(c + 128),
        .pos{x++, y},
        .alpha = a,
    });
  }
}

static void draw() {
  // TODO: credits
  // TODO: random play area on the background
  // SPLASH

  // MENU OPTIONS
  draw_str("NEW GAME", 1, 0);
  draw_str("CONTINUE", 1, 1, g_has_save ? 1.0 : 0.4);
  draw_str("OPTIONS", 1, 2);
  draw_str("EXIT", 1, 3);

  // MENU SELECTION
  qsu::blit(spr::minotaur, 0, g_sel, 0);
}

static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, ::draw); });
}

static void sel_up() {
  g_sel = static_cast<options>(g_sel - 1);
  if (g_sel < o_new_game)
    g_sel = o_exit;
  if (!g_has_save && g_sel == o_continue)
    g_sel = o_new_game;
  redraw();
}
static void sel_down() {
  g_sel = static_cast<options>(g_sel + 1);
  if (g_sel > o_exit)
    g_sel = o_new_game;
  if (!g_has_save && g_sel == o_continue)
    g_sel = o_options;
  redraw();
}

static void select() {
  switch (g_sel) {
  case o_new_game:
    save::reset();
    cno::modes::game();
    break;
  case o_continue:
    cno::modes::game();
    break;
  case o_options:
    cno::modes::options();
    break;
  case o_exit:
    casein::exit(0);
    break;
  }
}

void cno::modes::mainmenu() {
  using namespace casein;

  save::read().map([] { g_has_save = true; }).log_error();

  reset_k(KEY_DOWN);

  handle(KEY_DOWN, K_UP, sel_up);
  handle(KEY_DOWN, K_DOWN, sel_down);
  handle(KEY_DOWN, K_W, sel_up);
  handle(KEY_DOWN, K_S, sel_down);
  handle(KEY_DOWN, K_ENTER, select);

  redraw();

  quack::donald::push_constants({
      .grid_pos = {6, 0},
      .grid_size = {12, 12},
  });
}
