module cnossus;
import enemies;
import inv;
import light;
import play;
import player;
import sitime;
import spr;

static constexpr const float anim_length = 100;

static unsigned current_enemy{};
static dotz::ivec2 target{};
static sitime::stopwatch timer{};

static void check_next_enemy();

static void animate_move() {
  auto &e = enemies::d.list[current_enemy];

  auto dt = timer.millis() / anim_length;
  if (dt > 1.0) {
    e.anim_coord = e.coord = target;
    play::redraw();

    casein::handle(casein::REPAINT, nullptr);
    current_enemy++;
    check_next_enemy();
    return;
  }

  e.anim_coord = dotz::mix(e.coord, target, dt);
  play::redraw();
}

static void check_next_enemy() {
  for (; current_enemy < enemies::max_enemies; current_enemy++) {
    auto &e = enemies::d.list[current_enemy];

    if (e.spr == spr::nil || e.life == 0)
      continue;

    auto p =
        e.coord + enemies::next_move(e, player::coord(), light::d.charge > 0);
    if (!map::can_walk(p.x, p.y))
      continue;
    if (enemies::at(p))
      continue;

    if (player::coord() == p) {
      auto enemy_atk = life_of(e.spr);
      auto player_def = inv::defense() + player::defense();
      auto poison = poison_of(e.spr);
      player::hit(enemy_atk - player_def, poison);

      // casein::handle(casein::REPAINT, animate);
      continue;
    }

    auto da = dotz::abs(e.coord - player::coord());
    auto db = dotz::abs(p - player::coord());
    if (da.x > 1 && da.y > 1 && db.x > 1 && db.y > 1) {
      // Enemy wasn't and isn't visible. Don't animate it
      e.anim_coord = e.coord = p;
      continue;
    }

    timer = {};
    target = p;
    casein::handle(casein::REPAINT, animate_move);
    return;
  }

  play::redraw();
  player::d.life == 0 ? cno::modes::gameover() : cno::modes::game();
}

void cno::modes::enemy_turn::enter() {
  using namespace casein;
  reset_k(KEY_DOWN);
  handle(REPAINT, nullptr);

  light::tick();

  // TODO: animate poison damage
  player::poison_tick();
  if (player::d.life == 0)
    return cno::modes::gameover();

  current_enemy = 0;
  check_next_enemy();
}
