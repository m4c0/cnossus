module cnossus;
import dotz;
import inv;
import light;
import party;
import play;
import player;
import save;
import sfx;
import spr;
import timeline;

static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, play::draw); });
}

static void move_by(int dx, int dy) {
  tim::reset();

  auto p = player::coord() + dotz::ivec2{dx, dy};
  if (map::at(p.x, p.y) == spr::exit) {
    // TODO: animate via fade to black or similar
    sfx::next_level();
    cno::next_level();
    return;
  }
  if (!map::can_walk(p.x, p.y)) {
    sfx::fail();
    cno::modes::player_turn::attack(p);
    return;
  }

  play::move_by(p);
  redraw();
  player::d.life == 0 ? cno::modes::gameover() : cno::modes::timeline();
}

static void inv_l(int id) {
  auto roll = light_of(inv::burn_light(id));
  if (roll == 0)
    return;

  light::restore(roll);
  sfx::light();
  redraw();
}
static void inv_f(int id) {
  auto roll = food_of(inv::eat_food(id));
  if (roll == 0)
    return;

  player::restore(roll);
  sfx::eat();
  redraw();
}

static void enable_input() {
  using namespace casein;

  reset_k(KEY_DOWN);

  handle(KEY_DOWN, K_LEFT, [] { move_by(-1, 0); });
  handle(KEY_DOWN, K_RIGHT, [] { move_by(1, 0); });
  handle(KEY_DOWN, K_UP, [] { move_by(0, -1); });
  handle(KEY_DOWN, K_DOWN, [] { move_by(0, 1); });

  handle(KEY_DOWN, K_1, [] { inv_l(0); });
  handle(KEY_DOWN, K_2, [] { inv_l(1); });
  handle(KEY_DOWN, K_3, [] { inv_l(2); });
  handle(KEY_DOWN, K_4, [] { inv_f(0); });
  handle(KEY_DOWN, K_5, [] { inv_f(1); });
  handle(KEY_DOWN, K_6, [] { inv_f(2); });

  handle(KEY_DOWN, K_ESCAPE, [] {
    save::write();
    cno::modes::mainmenu();
  });

  handle(REPAINT, nullptr);
}

void cno::modes::game() {
  tim::reset();

  quack::donald::push_constants({
      .grid_pos = {},
      .grid_size = {9, 9},
  });

  enable_input();

  redraw();
}
