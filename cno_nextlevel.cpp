module cnossus;
import play;
import sfx;
import sitime;

static sitime::stopwatch timer{};

static void fade_in() {
  if (timer.millis() > 300) {
    cno::modes::game();
    return;
  }

  float f = timer.millis() / 300.0f;
  play::redraw(f);
}

static void change_level() {
  if (timer.millis() > 300) {
    timer = {};
    sfx::next_level(0.5);
    casein::handle(casein::REPAINT, fade_in);
    return;
  }

  // TODO: show player the bonus they got after a new level
}
static void fade_out() {
  if (timer.millis() > 300) {
    play::redraw(0.f);
    cno::next_level();
    sfx::next_level(0.8);
    timer = {};
    casein::handle(casein::REPAINT, change_level);
    return;
  }

  float f = timer.millis() / 300.0f;
  play::redraw(1.0 - f);
}

void cno::modes::nextlevel() {
  cno::reset_casein();

  timer = {};
  sfx::next_level(1);
  casein::handle(casein::REPAINT, fade_out);
}
