module cnossus;
import dotz;
import enemies;
import inv;
import lootroll;
import party;
import play;
import player;
import sitime;
import spr;
import sfx;

static constexpr const float anim_length = 200;

static qsu::anim *target;
static sitime::stopwatch timer{};
static float angle;

static void end() {
  player::d.anim_coord = player::d.coord;
  target->rotation = 0;

  if (party::is_animating())
    return;

  cno::modes::enemy_turn::enter(player::d.coord);
}

static void frame() {
  play::redraw();

  if (timer.millis() > anim_length)
    return end();

  float dt = timer.millis() / anim_length;
  if (dt > 0.5)
    dt = 1.0 - dt;

  player::d.anim_coord = dotz::mix(player::d.coord, target->coord, dt);
  target->rotation = dt * angle * 45.0f;
}

void cno::modes::player_turn::attack(enemies::enemy *e) {
  timer = {};
  target = e;

  auto player_atk = inv::attack() + player::attack();
  auto enemy_def = life_of(e->spr);
  auto roll = player_atk - enemy_def;
  if (roll <= 0) {
    // TODO: miss sfx
    angle = 0;
  } else {
    sfx::enemy_take_hit();
    angle = rng::rand(2) ? 1.0f : -1.0f;
    e->life -= rng::rand(roll);
  }

  if (e->life <= 0) {
    // TODO: inline animation, avoid particle
    party::emit({
        .sprite{
            .id = e->spr,
            .pos = e->coord,
        },
        .timeout = 500.0f,
    });
    *e = {{
        .coord = e->coord,
        .spr = lootroll(e->spr),
    }};
  }

  using namespace casein;
  reset_k(KEY_DOWN);
  handle(REPAINT, frame);
}

void cno::modes::player_turn::attack(qsu::anim *l) {
  timer = {};
  target = l;

  party::emit({
      .sprite{
          .id = l->spr,
          .pos = l->coord,
      },
      .timeout = 200,
  });
  sfx::break_jar();

  l->spr = lootroll(l->spr);

  using namespace casein;
  reset_k(KEY_DOWN);
  handle(REPAINT, frame);
}