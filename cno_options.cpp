module cnossus;
import dotz;
import spr;

namespace {
enum options {
  o_sound,
  o_fullscreen,
  o_back,
};
}

static options g_sel{};

static bool g_sound{true};

static void draw_check(bool b, int x, int y) {
  if (b) {
    qsu::blit(qsu::sprite{
        .id = spr::cursor,
        .pos = dotz::vec2{x, y} + 0.05,
        .size = dotz::vec2{1.f} - 0.1,
        .colour = {0.9f, 0.1f, 0.0f, 1.0f},
    });
  }
  qsu::blit(spr::ui_check, x, y, 0);
}

static void draw() {
  qsu::draw_str("SOUND", 1, 0);
  qsu::draw_str("FULLSCREEN", 1, 1);
  qsu::draw_str("BACK", 1, 2);

  qsu::blit(spr::minotaur, 0, g_sel, 0);

  draw_check(g_sound, 12, 0);
  draw_check(casein::is_fullscreen(), 12, 1);
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
    g_sound = !g_sound;
    redraw();
    break;
  case o_fullscreen:
    casein::set_fullscreen(!casein::is_fullscreen());
    redraw();
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
