export module timeline;
import dotz;
import hai;
import sitime;

namespace tim {
export constexpr const auto anim_dur_ms = 100.0;

struct lin {
  dotz::vec2 *target{};
  dotz::vec2 a{};
  dotz::vec2 b{};
  float start{};
  float length{};
};

hai::varray<lin> timeline{1000};
sitime::stopwatch timer{};

export void reset() {
  timer = {};
  timeline.truncate(0);
}
export void add(lin l) {
  // TODO: optimise for invisible sprites
  timeline.push_back(l);
}

export [[nodiscard]] bool tick() {
  float ms = timer.millis();
  bool animating{false};
  for (auto &l : timeline) {
    if (l.length == 0 || !l.target)
      continue;

    auto dt = (ms - l.start) / l.length;
    dt = dt < 0 ? 0 : (dt > 1 ? 1 : dt);
    *l.target = dotz::mix(l.a, l.b, dt);

    animating |= dt < 1;
  }
  return animating;
}
} // namespace tim
