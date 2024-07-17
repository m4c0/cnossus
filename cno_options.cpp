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
  qsu::draw_str("BACK", 1, 3);

  qsu::blit(spr::minotaur, g_sel, 0);
}
static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, ::draw); });
}

void cno::modes::options() {
  using namespace casein;

  reset_k(KEY_DOWN);

  handle(KEY_DOWN, K_ESCAPE, modes::mainmenu);

  redraw();
}
