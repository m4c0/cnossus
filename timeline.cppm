export module timeline;
import dotz;
import hai;
import sitime;

export namespace tim::fn {
using t = float (*)(float);

float linear(float dt) { return dt; }
float half_and_back(float dt) { return dt > 0.5 ? 1.0 - dt : dt; }
} // namespace tim::fn

namespace tim {
export constexpr const auto anim_dur_ms = 100.0;

template <typename T, T def = {}> struct param {
  T *target{};
  T a{def};
  T b{def};
  fn::t func{fn::linear};
};
struct lin {
  param<dotz::vec2> pos{};
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

template <typename T> void tick(param<T> &l, float dt) {
  if (l.target == nullptr)
    return;

  dt = l.func(dt);
  *l.target = dotz::mix(l.a, l.b, dt);
}
export [[nodiscard]] bool tick() {
  float ms = timer.millis();
  bool animating{false};
  for (auto &l : timeline) {
    if (l.length == 0)
      continue;

    auto dt = (ms - l.start) / l.length;
    dt = dt < 0 ? 0 : (dt > 1 ? 1 : dt);

    animating |= dt < 1;

    tick(l.pos, dt);
  }
  return animating;
}
} // namespace tim
