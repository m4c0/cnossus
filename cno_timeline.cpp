module cnossus;
import party;
import play;
import timeline;

static void redraw() {
  bool is_animating = tim::tick();

  play::redraw();

  is_animating |= party::is_animating();
  if (!is_animating)
    cno::modes::game();
}

static void animate() {
  using namespace casein;

  reset_k(KEY_DOWN);

  handle(REPAINT, redraw);
}

void cno::modes::timeline() { animate(); }
