module cnossus;
import enemies;
import inv;
import light;
import play;
import player;
import spr;

void cno::modes::enemy_turn::enter() {
  using namespace casein;
  reset_k(KEY_DOWN);
  handle(REPAINT, nullptr);

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

  play::redraw();
  player::d.life == 0 ? cno::modes::gameover() : cno::modes::timeline();
}
