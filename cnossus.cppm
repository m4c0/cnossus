#pragma leco app
#pragma leco add_impl cno_game
#pragma leco add_impl cno_mainmenu
#pragma leco add_impl cno_options
export module cnossus;

import casein;
import rng;
import quack;
import qsu;

namespace cno::modes {
void game();
void mainmenu();
void options();
}

struct init {
  init() {
    rng::seed();

    using namespace quack::donald;

    app_name("cnossus");
    max_quads(1024);

    clear_colour({0, 0, 0, 1});
    atlas(qsu::atlas);

    cno::modes::mainmenu();
  }
} i;
