#pragma leco app
#pragma leco add_resource "atlas.png"
export module mapview;

import casein;
import dotz;
import map;
import quack;
import voo;

static quack::donald::atlas_t *atlas(voo::device_and_queue *dq) {
  return new voo::sires_image("atlas.png", dq);
}

static unsigned data(quack::mapped_buffers all) { return map::draw(all); }

struct init {
  init() {
    map::gen();

    using namespace quack::donald;

    app_name("cnossus-mapview");
    max_quads(1024);

    dotz::vec2 grid_size{map::width, map::height};
    push_constants({
        .grid_pos = grid_size / 2.0f,
        .grid_size = grid_size,
    });
    atlas(::atlas);
    data(::data);
  }
} i;
