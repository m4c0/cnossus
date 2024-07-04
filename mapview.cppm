#pragma leco app
export module mapview;

import casein;
import dotz;
import enemies;
import loot;
import map;
import player;
import qsu;
import quack;

static dotz::vec2 cam{};
static int rad{1000};

static void draw() {
  map::draw(cam, rad, false);
  loot::draw(cam, rad);
  enemies::draw(cam, rad);
  player::draw();
}
static unsigned data(quack::mapped_buffers all) { return qsu::draw(all, draw); }

static unsigned int level = 1;
static void redraw() {
  player::init(level);
  enemies::init(level);
  loot::init(level);
  quack::donald::data(::data);
}
static void regen() {
  map::gen(level);
  redraw();
}

static void next_level() {
  level++;
  regen();
}
static void prev_level() {
  if (level > 1) {
    --level;
    regen();
  }
}

static void switch_light() {
  if (rad == 1000) {
    rad = 1;
    redraw();
  } else {
    rad = 1000;
    redraw();
  }
}

struct init {
  init() {
    using namespace casein;

    handle(KEY_DOWN, K_O, next_level);
    handle(KEY_DOWN, K_I, prev_level);
    handle(KEY_DOWN, K_P, regen);

    handle(KEY_DOWN, K_SPACE, switch_light);

    using namespace quack::donald;

    app_name("cnossus-mapview");
    max_quads(1024);

    dotz::vec2 grid_size{map::width, map::height};
    push_constants({
        .grid_pos = grid_size / 2.0f,
        .grid_size = grid_size,
    });
    atlas(qsu::atlas);
    regen();
  }
} i;
