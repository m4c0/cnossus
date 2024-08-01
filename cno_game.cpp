module cnossus;
import dotz;
import enemies;
import inv;
import light;
import loot;
import lootroll;
import party;
import play;
import player;
import save;
import sfx;
import spr;
import timeline;

static void take_loot(auto *l) {
  switch (l->spr) {
  case spr::jar:
  case spr::coffer:
    cno::modes::player_turn::attack(l);
    break;
  default:
    // TODO: better animation
    if (inv::take(l->spr)) {
      party::emit({
          .sprite{
              .id = l->spr,
              .pos = l->coord,
          },
          .timeout = 200,
      });
      sfx::pick();
      l->spr = spr::nil;
    }

    cno::modes::player_turn::move(l->coord);
    break;
  }
}

static void move_by(int dx, int dy) {
  auto p = player::coord() + dotz::ivec2{dx, dy};
  if (map::at(p.x, p.y) == spr::exit) {
    // TODO: show player the bonus they got after a new level
    // TODO: animate via fade to black or similar
    sfx::next_level();
    cno::next_level();
    play::redraw();
    return;
  }
  if (!map::can_walk(p.x, p.y)) {
    sfx::fail();
    cno::modes::player_turn::fail(p);
    return;
  }

  if (auto *e = enemies::at(p)) {
    if (e->life > 0 && e->spr != spr::nil) {
      cno::modes::player_turn::attack(e);
      return;
    }
    if (e->spr != spr::nil) {
      take_loot(e);
      return;
    }
  }
  if (auto *l = loot::at(p)) {
    take_loot(l);
    return;
  }

  cno::modes::player_turn::move(p);
}

static void inv_l(int id) {
  auto roll = light_of(inv::burn_light(id));
  if (roll == 0)
    return;

  light::restore(roll);
  sfx::light();
  play::redraw();
}
static void inv_f(int id) {
  auto roll = food_of(inv::eat_food(id));
  if (roll == 0)
    return;

  player::restore(roll);
  sfx::eat();
  play::redraw();
}

void cno::modes::game() {
  cno::reset_casein();

  using namespace casein;

  handle(KEY_DOWN, K_LEFT, [] { move_by(-1, 0); });
  handle(KEY_DOWN, K_RIGHT, [] { move_by(1, 0); });
  handle(KEY_DOWN, K_UP, [] { move_by(0, -1); });
  handle(KEY_DOWN, K_DOWN, [] { move_by(0, 1); });

  handle(KEY_DOWN, K_A, [] { move_by(-1, 0); });
  handle(KEY_DOWN, K_D, [] { move_by(1, 0); });
  handle(KEY_DOWN, K_W, [] { move_by(0, -1); });
  handle(KEY_DOWN, K_S, [] { move_by(0, 1); });

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

  handle(GESTURE, G_SWIPE_LEFT, [] { move_by(-1, 0); });
  handle(GESTURE, G_SWIPE_RIGHT, [] { move_by(1, 0); });
  handle(GESTURE, G_SWIPE_UP, [] { move_by(0, -1); });
  handle(GESTURE, G_SWIPE_DOWN, [] { move_by(0, 1); });

  quack::donald::push_constants({
      .grid_pos = {},
      .grid_size = {9, 9},
  });

  play::redraw();
}
