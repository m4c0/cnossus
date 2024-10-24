#ifndef LECO_TARGET_IOS
#pragma leco app
#endif

import casein;
import map;
import play;
import qsu;
import quack;
import save;

struct init {
  init() {
    using namespace quack::donald;

    app_name("cnossus");
    max_quads(map::width * map::height * 2 + 1024);

    clear_colour({0, 0, 0, 1});
    atlas(qsu::atlas);

    save::read([] {}, [] {});

    play::redraw();
  }
} i;
