#pragma leco app
#pragma leco add_impl cno_enemy_turn
#pragma leco add_impl cno_game
#pragma leco add_impl cno_gameover
#pragma leco add_impl cno_level
#pragma leco add_impl cno_mainmenu
#pragma leco add_impl cno_options
#pragma leco add_impl cno_player_attack
#pragma leco add_impl cno_player_turn
export module cnossus;

import casein;
import dotz;
import map;
import rng;
import quack;
import qsu;

namespace enemies {
class enemy;
}

namespace cno::modes {
void game();
void gameover();
void mainmenu();
void options();
void timeline();
} // namespace cno::modes

namespace cno::modes::player_turn {
void attack(enemies::enemy *e);
void attack(qsu::anim *l);
void fail(dotz::ivec2 target);
void move(dotz::ivec2 target);
} // namespace cno::modes::player_turn

namespace cno::modes::enemy_turn {
void enter();
}

namespace cno {
void next_level();
void load_options();

void reset_casein() {
  using namespace casein;

  reset_k(KEY_DOWN);

  handle(REPAINT, nullptr);
}
} // namespace cno

// TODO: show some progress (show game level?)
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
