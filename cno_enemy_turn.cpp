module cnossus;
import enemies;
import inv;
import light;
import play;
import player;
import sfx;
import sitime;
import spr;

static constexpr const float anim_length = 300;

static unsigned current_enemy{};
static dotz::ivec2 target{};
static sitime::stopwatch timer{};
static unsigned angle{};

static void check_next_enemy();

static void end_enemy_animation(dotz::ivec2 p) {
  auto &e = enemies::d.list[current_enemy];

  player::d.rotation = 0;
  e.anim_coord = e.coord = p;
  play::redraw();

  casein::handle(casein::REPAINT, nullptr);
  current_enemy++;
  check_next_enemy();
}

static void animate_move() {
  auto &e = enemies::d.list[current_enemy];

  auto dt = timer.millis() / anim_length;
  if (dt > 1.0)
    return end_enemy_animation(target);

  e.anim_coord = dotz::mix(e.coord, target, dt);
  play::redraw();
}

static void animate_attack() {
  auto &e = enemies::d.list[current_enemy];

  auto dt = timer.millis() / anim_length;
  if (dt > 1.0)
    return end_enemy_animation(e.coord);

  if (dt > 0.5)
    dt = 1.0 - dt;

  e.anim_coord = dotz::mix(e.coord, target, dt);
  player::d.rotation = dt * angle * 45.0f;
  play::redraw();
}

static void check_next_enemy() {
  for (; current_enemy < enemies::max_enemies; current_enemy++) {
    auto &e = enemies::d.list[current_enemy];

    if (e.spr == spr::nil || e.life == 0)
      continue;

    auto p = target =
        e.coord + enemies::next_move(e, player::coord(), light::d.charge > 0);
    if (!map::can_walk(p.x, p.y))
      continue;
    if (enemies::at(p))
      continue;

    if (player::coord() == p) {
      auto poison = poison_of(e.spr);
      if (poison > 0) {
        player::d.poison += rng::rand(poison);
        sfx::poison();
      }

      auto enemy_atk = life_of(e.spr);
      auto player_def = inv::defense() + player::defense();
      auto roll = enemy_atk - player_def;
      if (roll <= 0) {
        angle = 0;
        sfx::attack_miss();
      } else {
        angle = rng::rand(2) ? 1 : -1;

        sfx::player_take_hit();
        player::d.life -= rng::rand(roll);
        if (player::d.life < 0)
          player::d.life = 0;
      }

      timer = {};
      return casein::handle(casein::REPAINT, animate_attack);
    }

    auto da = dotz::abs(e.coord - player::coord());
    auto db = dotz::abs(p - player::coord());
    if ((da.x > 1 || da.y > 1) && (db.x > 1 || db.y > 1)) {
      // Enemy wasn't and isn't visible. Don't animate it
      e.anim_coord = e.coord = p;
      continue;
    }

    timer = {};
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
