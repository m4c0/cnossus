module cnossus;
import play;
import player;
import sitime;

static constexpr const float anim_length = 100;

static dotz::ivec2 target{};
static sitime::stopwatch timer{};
static void (*on_end)();
static void (*on_frame)();

static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, play::draw); });

  if (timer.millis() > anim_length) {
    on_end();
    cno::modes::game();
  } else {
    on_frame();
  }
}

void cno::modes::player_turn::attack(dotz::ivec2 tgt) {
  timer = {};
  target = tgt;

  on_end = [] { player::d.anim_coord = player::d.coord; };
  on_frame = [] {
    float dt = timer.millis() / anim_length;
    if (dt > 0.5)
      dt = 1.0 - dt;
    dt *= 0.5;
    player::d.anim_coord = dotz::mix(player::d.coord, target, dt);
  };

  using namespace casein;
  reset_k(KEY_DOWN);
  handle(REPAINT, redraw);
}
