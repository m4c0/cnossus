#pragma leco app
export module cnossus;

import casein;
import dotz;
import play;
import map;
import rng;
import quack;
import qsu;
import sitime;
import voo;

// TODO: animations (attack, player move, inventory)
// TODO: smooth camera moving

static void enable_input();
static void animate(int ms_timeout);

static unsigned int level = 1;
static sitime::stopwatch g_watch{};
static int g_timeout{};

static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, play::draw); });
}

static void load_level() {
  play::init(level);
  redraw();
}

static void move_by(int x, int y) { animate(play::move_by(x, y)); }

static void inv_l(int id) {
  play::light(id);
  animate(300);
}
static void inv_f(int id) {
  play::eat(id);
  animate(300);
}

static void enable_input() {
  using namespace casein;

  g_timeout = 0;

  handle(KEY_DOWN, K_LEFT, [] { move_by(-1, 0); });
  handle(KEY_DOWN, K_RIGHT, [] { move_by(1, 0); });
  handle(KEY_DOWN, K_UP, [] { move_by(0, -1); });
  handle(KEY_DOWN, K_DOWN, [] { move_by(0, 1); });

  handle(KEY_DOWN, K_1, [] { inv_l(0); });
  handle(KEY_DOWN, K_2, [] { inv_l(1); });
  handle(KEY_DOWN, K_3, [] { inv_l(2); });
  handle(KEY_DOWN, K_4, [] { inv_f(0); });
  handle(KEY_DOWN, K_5, [] { inv_f(1); });
  handle(KEY_DOWN, K_6, [] { inv_f(2); });

  handle(KEY_DOWN, K_SPACE, [] {
    level = 1;
    load_level();
  });

  handle(REPAINT, [] {});
}
static void animate(int ms_timeout) {
  using namespace casein;

  g_timeout = ms_timeout;
  g_watch = {};

  if (ms_timeout == 0) {
    redraw();
    return;
  }

  reset_k(KEY_DOWN);

  handle(REPAINT, [] {
    redraw();

    if (g_timeout > 0 && g_watch.millis() > g_timeout)
      enable_input();
  });
}

struct init {
  init() {
    rng::seed();
    load_level();
    enable_input();

    using namespace quack::donald;

    app_name("cnossus");
    max_quads(1024);

    clear_colour({0, 0, 0, 1});
    atlas(qsu::atlas);
    redraw();
  }
} i;
