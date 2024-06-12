#pragma leco app
export module cnossus;

import casein;
import dotz;
import play;
import map;
import quack;
import voo;

static unsigned data(quack::mapped_buffers all) { return play::draw(all); }

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

struct init {
  init() {
    load_level();

    using namespace casein;

    handle(KEY_DOWN, K_LEFT, left);
    handle(KEY_DOWN, K_RIGHT, right);
    handle(KEY_DOWN, K_UP, up);
    handle(KEY_DOWN, K_DOWN, down);

    using namespace quack::donald;

    app_name("cnossus");
    max_quads(1024);

    dotz::vec2 grid_size{map::width, map::height};
    push_constants({
        .grid_pos = grid_size / 2.0f,
        .grid_size = grid_size,
    });
    atlas(play::atlas);
    data(::data);
  }
} i;
