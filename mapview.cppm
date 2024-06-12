#pragma leco app
#pragma leco add_resource "atlas.png"
export module mapview;

import casein;
import dotz;
import play;
import map;
import quack;
import voo;

static unsigned data(quack::mapped_buffers all) { return play::draw(all); }

static unsigned int level = 1;
static void regen() {
  play::init(level);
  quack::donald::data(::data);
}

static void next_level() {
  level++;
  regen();
}
static void prev_level() {
  if (level > 1)
    --level;
  regen();
}

struct init {
  init() {
    using namespace casein;

    handle(KEY_DOWN, K_RIGHT, next_level);
    handle(KEY_DOWN, K_LEFT, prev_level);
    handle(KEY_DOWN, K_DOWN, regen);

    using namespace quack::donald;

    app_name("cnossus-mapview");
    max_quads(1024);

    dotz::vec2 grid_size{map::width, map::height};
    push_constants({
        .grid_pos = grid_size / 2.0f,
        .grid_size = grid_size,
    });
    atlas(play::atlas);
    regen();
  }
} i;
