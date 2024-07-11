module cnossus;

namespace {
enum options {
  o_new_game,
  o_continue,
  o_options,
  o_exit,
};
}
static options g_sel{};

static void draw() {
  // TODO: credits
  // TODO: random play area on the background
  // SPLASH
  // MENU OPTIONS
  // MENU SELECTION
}

static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, ::draw); });
}

static void sel_up() {
  g_sel = static_cast<options>(g_sel - 1);
  if (g_sel < o_new_game)
    g_sel = o_exit;
  // TODO: test continue
}
static void sel_down() {
  g_sel = static_cast<options>(g_sel + 1);
  if (g_sel > o_exit)
    g_sel = o_new_game;
  // TODO: test continue
}

static void select() {
  // TODO: run selection
}

void cno::modes::mainmenu() {
  using namespace casein;

  reset_k(KEY_DOWN);

  handle(KEY_DOWN, K_UP, sel_up);
  handle(KEY_DOWN, K_DOWN, sel_down);
  handle(KEY_DOWN, K_W, sel_up);
  handle(KEY_DOWN, K_S, sel_down);
  handle(KEY_DOWN, K_ENTER, select);

  redraw();
}
