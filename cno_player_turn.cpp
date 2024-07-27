module cnossus;
import play;
import sitime;

static constexpr const auto anim_length = 1000;

static sitime::stopwatch m_timer{};

static void redraw() {
  quack::donald::data([](auto all) { return qsu::draw(all, play::draw); });

  if (m_timer.millis() > anim_length)
    cno::modes::game();
}

void cno::modes::player_turn::attack() {
  m_timer = {};

  using namespace casein;
  reset_k(KEY_DOWN);
  handle(REPAINT, redraw);
}
