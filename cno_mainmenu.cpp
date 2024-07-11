module cnossus;

// TODO: gen random maps on the background

static void draw() {}

static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, ::draw); });
}

void cno::modes::mainmenu() {
  using namespace casein;

  reset_k(KEY_DOWN);

  redraw();
}
