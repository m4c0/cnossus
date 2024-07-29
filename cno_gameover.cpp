module cnossus;
import party;
import play;
import player;
import save;
import spr;

static void redraw() {
  // TODO: hide UI
  // TODO: fade lights
  // TODO: sad song
  play::redraw();

  if (!party::is_animating()) {
    using namespace casein;
    handle(KEY_DOWN, K_ESCAPE, cno::modes::mainmenu);
    handle(REPAINT, nullptr);
  }
}

void cno::modes::gameover() {
  using namespace casein;

  party::emit({
      .sprite{
          .id = spr::minotaur,
          .pos = player::d.coord,
      },
      .timeout = 1000,
  });

  cno::reset_casein();
  handle(REPAINT, redraw);

  save::clear();
}
