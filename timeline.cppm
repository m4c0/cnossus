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
float end{};

export void reset() {
  timer = {};
  timeline.truncate(0);
  end = {};
}
export void add(lin l) {
  timeline.push_back(l);
  end = dotz::max(end, l.start + l.length);
}

export void tick() {
  float ms = timer.millis();
  for (auto &l : timeline) {
    if (l.length == 0 || !l.target)
      continue;

    auto dt = (ms - l.start) / l.length;
    dt = dt < 0 ? 0 : (dt > 1 ? 1 : dt);
    *l.target = dotz::mix(l.a, l.b, dt);
  }
}

export bool is_animating() { return end > 0 && end > timer.millis(); }
} // namespace tim
