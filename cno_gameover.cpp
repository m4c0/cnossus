module cnossus;
import play;

static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, play::draw); });
}

void cno::modes::gameover() {
  using namespace casein;

  reset_k(KEY_DOWN);
  handle(REPAINT, nullptr);

  // TODO: hide UI
  // TODO: fade lights
  // TODO: sad song
  redraw();
}
