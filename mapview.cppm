#pragma leco app
#pragma leco add_resource "atlas.png"
export module mapview;

import casein;
import dotz;
import enemies;
import map;
import player;
import quack;
import voo;

static quack::donald::atlas_t *atlas(voo::device_and_queue *dq) {
  return new voo::sires_image("atlas.png", dq);
}

static unsigned data(quack::mapped_buffers all) {
  auto count = map::draw(all);
  count += enemies::draw(all);
  count += player::draw(all);
  return count;
}

static unsigned int level = 1;
static void regen() {
  map::gen(level);
  player::init(level);
  enemies::init(level);
  quack::donald::data(::data);
  level++;
}

struct init {
  init() {
    using namespace casein;

    handle(KEY_DOWN, K_SPACE, regen);

    using namespace quack::donald;

    app_name("cnossus-mapview");
    max_quads(1024);

    dotz::vec2 grid_size{map::width, map::height};
    push_constants({
        .grid_pos = grid_size / 2.0f,
        .grid_size = grid_size,
    });
    atlas(::atlas);
    regen();
  }
} i;
