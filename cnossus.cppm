#pragma leco app
#pragma leco add_impl cno_enemy_turn
#pragma leco add_impl cno_game
#pragma leco add_impl cno_gameover
#pragma leco add_impl cno_level
#pragma leco add_impl cno_mainmenu
#pragma leco add_impl cno_options
#pragma leco add_impl cno_player_turn
#pragma leco add_impl cno_timeline
export module cnossus;

import casein;
import dotz;
import map;
import rng;
import quack;
import qsu;

namespace cno::modes {
void game();
void gameover();
void mainmenu();
void options();
void timeline();
} // namespace cno::modes

namespace cno::modes::player_turn {
void attack(dotz::ivec2 target);
void move(dotz::ivec2 target);
} // namespace cno::modes::player_turn
namespace cno::modes::enemy_turn {
void enter(dotz::ivec2 player);
}

namespace cno {
void next_level();
void load_options();
} // namespace cno

// TODO: add sounds
struct init {
  init() {
    rng::seed();

    cno::load_options();

    using namespace casein;
    window_title = "Escape from Cnossus";
    window_size = {800, 800};

    using namespace quack::donald;

    app_name("cnossus");
    max_quads(map::width * map::height * 2 + 1024);

    clear_colour({0, 0, 0, 1});
    atlas(qsu::atlas);

    cno::modes::mainmenu();
  }
} i;
