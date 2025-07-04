#ifndef LECO_TARGET_IOS
#pragma leco app
#endif
export module mapview;
import anim;
import casein;
import dotz;
import enemies;
import loot;
import map;
import player;
import qsu;

static dotz::vec2 cam{};
static int rad{1000};

static void draw() {
  anim::fog_realign = false;
  anim::fog_center = cam;
  anim::fog_radius = rad;
  map::draw();
  loot::draw();
  enemies::draw();
  player::draw();
}

static unsigned int level = 1;
static void redraw() {
  qsu::draw(draw);
}
static void regen() {
  map::gen(level);
  player::init(level);
  enemies::init(level);
  loot::init(level);
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
static void move_light(float dx, float dy) {
  cam.x += dx * 0.25;
  cam.y += dy * 0.25;
  redraw();
}

struct init {
  init() {
    using namespace casein;

    handle(KEY_DOWN, K_O, next_level);
    handle(KEY_DOWN, K_I, prev_level);
    handle(KEY_DOWN, K_P, regen);

    handle(KEY_DOWN, K_SPACE, switch_light);
    handle(KEY_DOWN, K_LEFT, [] { move_light(-1, 0); });
    handle(KEY_DOWN, K_RIGHT, [] { move_light(1, 0); });
    handle(KEY_DOWN, K_UP, [] { move_light(0, -1); });
    handle(KEY_DOWN, K_DOWN, [] { move_light(0, 1); });

    dotz::vec2 grid_size{map::width, map::height};
    qsu::grid_size({
        .grid_pos = grid_size / 2.0f,
        .grid_size = grid_size,
    });
    qsu::init();
    regen();
  }
} i;
