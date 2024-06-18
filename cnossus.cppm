#pragma leco app
export module cnossus;

import casein;
import dotz;
import play;
import map;
import rng;
import quack;
import qsu;
import voo;

// TODO: animations
// TODO: smooth camera moving

static unsigned data(auto all) { return qsu::draw(all, play::draw); }

static unsigned int level = 1;

static void load_level() {
  play::init(level);
  quack::donald::data(::data);
}
static void left() {
  play::move_by(-1, 0);
  quack::donald::data(::data);
}
static void right() {
  play::move_by(1, 0);
  quack::donald::data(::data);
}
static void up() {
  play::move_by(0, -1);
  quack::donald::data(::data);
}
static void down() {
  play::move_by(0, 1);
  quack::donald::data(::data);
}

static void inv_l(int id) {
  play::light(id);
  quack::donald::data(::data);
}
static void inv_f(int id) {
  play::eat(id);
  quack::donald::data(::data);
}

struct init {
  init() {
    rng::seed();
    load_level();

    using namespace casein;

    handle(KEY_DOWN, K_LEFT, left);
    handle(KEY_DOWN, K_RIGHT, right);
    handle(KEY_DOWN, K_UP, up);
    handle(KEY_DOWN, K_DOWN, down);

    handle(KEY_DOWN, K_1, [] { inv_l(0); });
    handle(KEY_DOWN, K_2, [] { inv_l(1); });
    handle(KEY_DOWN, K_3, [] { inv_l(2); });
    handle(KEY_DOWN, K_4, [] { inv_f(3); });
    handle(KEY_DOWN, K_5, [] { inv_f(4); });
    handle(KEY_DOWN, K_6, [] { inv_f(5); });

    handle(KEY_DOWN, K_SPACE, [] {
      level = 1;
      load_level();
    });

    using namespace quack::donald;

    app_name("cnossus");
    max_quads(1024);

    atlas(play::atlas);
    data(::data);
  }
} i;
