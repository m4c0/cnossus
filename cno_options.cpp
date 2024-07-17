module cnossus;

namespace {
enum options {
  o_sound,
  o_fullscreen,
  o_back,
};
}

static options g_sel{};

static void draw() {
  qsu::draw_str("SOUND", 1, 0);
  qsu::draw_str("FULLSCREEN", 1, 1);
  qsu::draw_str("BACK", 1, 2);

  qsu::blit(spr::minotaur, 0, g_sel, 0);
}
static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, ::draw); });
}

static void sel_up() {
  g_sel = static_cast<options>(g_sel - 1);
  if (g_sel < o_sound)
    g_sel = o_back;
  redraw();
}
static void sel_down() {
  g_sel = static_cast<options>(g_sel + 1);
  if (g_sel > o_back)
    g_sel = o_sound;
  redraw();
}

static void select() {
  switch (g_sel) {
  case o_sound:
  case o_fullscreen:
    break;
  case o_back:
    cno::modes::mainmenu();
    break;
  }
}

void cno::modes::options() {
  using namespace casein;

  reset_k(KEY_DOWN);

  handle(KEY_DOWN, K_ESCAPE, modes::mainmenu);

  handle(KEY_DOWN, K_UP, sel_up);
  handle(KEY_DOWN, K_DOWN, sel_down);
  handle(KEY_DOWN, K_W, sel_up);
  handle(KEY_DOWN, K_S, sel_down);
  handle(KEY_DOWN, K_ENTER, select);

  redraw();
}
