module cnossus;
import play;
import player;
import sitime;

static constexpr const float anim_length = 100;

static dotz::ivec2 target{};
static sitime::stopwatch timer{};

static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, play::draw); });
}

void cno::modes::player_turn::attack(dotz::ivec2 tgt) {
  timer = {};
  target = tgt;

  using namespace casein;
  reset_k(KEY_DOWN);
  handle(REPAINT, [] {
    redraw();

    if (timer.millis() > anim_length) {
      player::d.anim_coord = player::d.coord;
      cno::modes::enemy_turn::enter(player::d.coord);
      return;
    }

    float dt = timer.millis() / anim_length;
    if (dt > 0.5)
      dt = 1.0 - dt;
    dt *= 0.5;
    player::d.anim_coord = dotz::mix(player::d.coord, target, dt);
  });
}
