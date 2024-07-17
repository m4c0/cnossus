module cnossus;

static void draw() {}
static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, ::draw); });
}

void cno::modes::options() {
  using namespace casein;

  reset_k(KEY_DOWN);

  handle(KEY_DOWN, K_ESCAPE, modes::mainmenu);

  redraw();
}
