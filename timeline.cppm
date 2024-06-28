export module timeline;
import dotz;
import hai;
import sitime;

namespace tim {
struct lin {
  dotz::vec2 *target{};
  dotz::vec2 a{};
  dotz::vec2 b{};
  float start{};
  float length{};
};

hai::varray<lin> timeline{1000};
sitime::stopwatch timer{};

void reset() {
  timer = {};
  timeline.truncate(0);
}
void add(lin l) { timeline.push_back(l); }

void tick() {
  auto ms = timer.millis();
  for (auto &l : timeline) {
    if (l.length == 0 || !l.target)
      continue;

    auto dt = (ms - l.start) / l.length;
    if (dt < 0)
      continue;

    *l.target = dotz::mix(l.a, l.b, dt);
  }
}

bool is_animating() {
  auto ms = timer.millis();
  for (auto &l : timeline) {
    if (ms - l.start < l.length)
      return true;
  }
  return false;
}
} // namespace tim
