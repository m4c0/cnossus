#pragma leco app
#pragma leco app_id "br.com.tpk.Cnossus"
#pragma leco app_version "2.0.0"
#pragma leco add_impl cno_enemy_turn
#pragma leco add_impl cno_game
#pragma leco add_impl cno_gameover
#pragma leco add_impl cno_level
#pragma leco add_impl cno_nextlevel
#pragma leco add_impl cno_player_attack
#pragma leco add_impl cno_player_turn
export module cnossus;

#ifdef LECO_TARGET_IOS
#pragma leco add_impl cno_mainmenu_ios
#else
#pragma leco add_impl cno_mainmenu
#pragma leco add_impl cno_options
#endif

import anim;
import casein;
import dotz;
import enemies;
import map;
import rng;
import qsu;

namespace cno::modes {
void game();
void gameover();
void nextlevel();
void mainmenu();
void options();
void timeline();
} // namespace cno::modes

namespace cno::modes::player_turn {
void attack(enemies::enemy *e);
void attack(anim::t *l);
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
  reset_g(GESTURE);

  handle(REPAINT, {});
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

    qsu::init();
    cno::modes::mainmenu();
  }
} i;
