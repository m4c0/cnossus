export module play;
import dotz;
import enemies;
import inv;
import light;
import loot;
import lootroll;
import map;
import party;
import player;
import qsu;
import save;
import sfx;
import spr;

// TODO: fix phantom loot when enemies (and chests) die
namespace play {
export void next_level() {
  auto level = ++save::d.level;
  map::gen(level);
  player::init(level);
  enemies::init(level);
  loot::init(level);
  light::init(level);
  inv::init(level);
  save::write();
}

export void draw() {
  auto pac = player::anim_coord();
  auto radius = light::d.charge > 0 ? 2 : 1;
  {
    qsu::guard::position ppo{-(pac + 0.5f)};
    map::draw(pac, radius);
    loot::draw(pac, radius);
    enemies::draw(pac, radius);
    player::draw();
    party::draw();
  }

  player::draw_ui();
  light::draw_ui();
  inv::draw_ui();
}

static void take_loot(auto *l) {
  switch (l->spr) {
  case spr::jar:
  case spr::coffer:
    party::emit({
        .sprite{
            .id = l->spr,
            .pos = l->coord,
        },
        .timeout = 200,
    });
    sfx::break_jar();
    l->spr = lootroll(save::d.level, l->spr);
    player::attack(l->coord);
    break;
  default:
    player::move(l->coord);

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
    break;
  }
}

static void player_turn(dotz::ivec2 p) {
  player::poison_tick();
  light::tick();

  if (auto *e = enemies::at(p)) {
    if (e->life > 0) {
      auto player_atk = inv::attack() + player::attack();
      auto enemy_def = life_of(e->spr);
      enemies::hit(*e, player_atk - enemy_def);
      player::attack(e->coord);
    } else if (e->spr != spr::nil) {
      take_loot(e);
    }
  } else if (auto *l = loot::at(p)) {
    take_loot(l);
  } else {
    player::move(p);
  }
}

static void enemy_turn(dotz::ivec2 p) {
  for (auto &e : enemies::d.list) {
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
      enemies::attack(e, p);
      continue;
    }

    enemies::move(e, p);
  }
}

export void move_by(int dx, int dy) {
  auto p = player::coord() + dotz::ivec2{dx, dy};
  if (map::at(p.x, p.y) == spr::exit) {
    sfx::next_level();
    next_level();
    return;
  }
  if (!map::can_walk(p.x, p.y)) {
    sfx::fail();
    return;
  }

  player_turn(p);
  enemy_turn(p);
}
} // namespace play
