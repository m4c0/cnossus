#pragma leco app
#pragma leco add_impl cno_game
export module cnossus;

import casein;
import quack;
import qsu;

namespace cno::modes {
void game();
}

struct init {
  init() {
    using namespace quack::donald;

    app_name("cnossus");
    max_quads(1024);

    clear_colour({0, 0, 0, 1});
    atlas(qsu::atlas);

    cno::modes::game();
  }
} i;
