module cnossus;
import hai;
import jute;
import play;
import quack;
import qsu;
import save;
import sfx;
import sitime;

static sitime::stopwatch timer{};

static hai::cstr level = jute::view{"NEXT LEVEL"}.cstr();
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

static void fade_in() {
  if (timer.millis() > 300) {
    cno::modes::game();
    return;
  }

  float f = timer.millis() / 300.0f;
  play::redraw(f);
}

static void draw_level() {
  float x = level.size() / 2.0f;
  quack::donald::push_constants({
      .grid_pos = {x, 0.5},
      .grid_size = {20},
  });

  qsu::draw_str(level, 0, 0);
}
static void change_level() {
  if (timer.millis() > 300) {
    timer = {};
    sfx::next_level(0.5);
    casein::handle(casein::REPAINT, fade_in);
    return;
  }

  // TODO: show player the bonus they got after a new level

  quack::donald::data([](auto all) { return qsu::draw(all, draw_level); });
}
static void fade_out() {
  if (timer.millis() > 300) {
    play::redraw(0.f);
    cno::next_level();
    sfx::next_level(0.8);
    timer = {};
    level = make_lvl_str();
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
