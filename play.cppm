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
import quack;
import save;
import sfx;
import spr;

// TODO: fix phantom loot when enemies (and chests) die
namespace play {
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
export void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, draw); });
}

static void enemy_turn(dotz::ivec2 p) {
  player::poison_tick();
  light::tick();

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

export void move_by(dotz::ivec2 p) { enemy_turn(p); }
} // namespace play
