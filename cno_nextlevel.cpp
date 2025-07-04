module cnossus;
import hai;
import jute;
import play;
import qsu;
import save;
import sfx;
import sitime;
import ui;

static sitime::stopwatch timer{};
static float inter_fade{};

static hai::cstr level_label = jute::view{"NEXT LEVEL"}.cstr();
static auto make_lvl_str() {
  static constexpr const jute::view units[10]{
      "", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX",
  };
  static constexpr const jute::view decs[10]{
      "", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC",
  };
  static constexpr const jute::view cents[10]{
      "", "C", "CC", "CCC", "CM", "M", "MC", "MCC", "MCCC", "CM",
  };
  if (save::d.level >= 1000)
    return jute::view{"WTF"}.cstr();

  auto unit = units[save::d.level % 10];
  auto dec = decs[(save::d.level / 10) % 10];
  auto cent = cents[(save::d.level / 100) % 10];
  auto txt = "LEVEL " + cent + dec + unit;
  return txt.cstr();
}

static void draw_inter() {
  float x = level_label.size() / 2.0f;
  qsu::grid_size({
      .grid_pos = {x, 0.5},
      .grid_size = {20},
  });

  ui::draw_str(level_label, 0, 0, inter_fade);
  // TODO: show player the bonus they got after a new level
}

static void fade_in() {
  if (timer.millis() > 300) {
    cno::modes::game();
    return;
  }

  float f = timer.millis() / 300.0f;
  play::redraw(f);
}
static void fade_inter_out() {
  if (timer.millis() > 300) {
    timer = {};
    casein::handle(casein::REPAINT, fade_in);
    return;
  }

  inter_fade = 1.0f - (timer.millis() / 300.0f);
  qsu::draw(draw_inter);
}
static void show_inter() {
  if (timer.millis() > 500) {
    timer = {};
    casein::handle(casein::REPAINT, fade_inter_out);
    return;
  }

  inter_fade = 1;
  qsu::draw(draw_inter);
}
static void fade_inter_in() {
  if (timer.millis() > 300) {
    timer = {};
    sfx::next_level(0.5);
    casein::handle(casein::REPAINT, show_inter);
    return;
  }

  inter_fade = timer.millis() / 300.0f;
  qsu::draw(draw_inter);
}
static void fade_out() {
  if (timer.millis() > 300) {
    play::redraw(0.f);
    cno::next_level();
    sfx::next_level(0.8);
    timer = {};
    level_label = make_lvl_str();
    casein::handle(casein::REPAINT, fade_inter_in);
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
