module cnossus;
import anim;
import dice;
import dotz;
import enemies;
import inv;
import loot;
import lootroll;
import party;
import play;
import player;
import save;
import sitime;
import spr;
import sfx;

static constexpr const float anim_length = 200;

static anim::t *target;
static sitime::stopwatch timer{};
static float angle;

static void end() {
  player::d.anim_coord = player::d.coord;
  target->rotation = 0;

  if (party::is_animating())
    return;

  cno::modes::enemy_turn::enter();
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
  auto enemy_def = life_of(e->spr) + save::d.level / 2;
  auto roll = attack_roll(player_atk, enemy_def);
  e->life -= roll;
  angle = (roll == 0) ? 0 : rng::rand(2) ? 1.0f : -1.0f;

  if (e->life <= 0) {
    // TODO: inline animation, avoid particle
    party::emit({
        .sprite{
            .coord = e->coord,
            .spr = e->spr,
        },
        .timeout = 300.0f,
    });
    loot::add(e->coord, lootroll(e->spr));
    *e = {{
        .coord = e->coord,
        .anim_coord = e->coord,
        .spr = spr::splat,
    }};
  }

  using namespace casein;
  reset_k(KEY_DOWN);
  handle(REPAINT, frame);
}

void cno::modes::player_turn::attack(anim::t *l) {
  timer = {};
  target = l;

  party::emit({
      .sprite{
          .coord = l->coord,
          .spr = l->spr,
      },
      .timeout = 200,
  });
  sfx::break_jar();

  l->spr = lootroll(l->spr);

  cno::reset_casein();

  using namespace casein;
  handle(casein::REPAINT, frame);
}
