module cnossus;
import play;

void cno::modes::gameover() {
  using namespace casein;

  reset_k(KEY_DOWN);
  handle(REPAINT, nullptr);

  // TODO: hide UI
  // TODO: fade lights
  // TODO: sad song
  play::redraw();
}
