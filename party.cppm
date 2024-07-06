export module party;

import dotz;
import hai;
import qsu;
import sitime;
import spr;

namespace party {
struct particle {
  qsu::sprite sprite;
  float timeout;
  float started;
};

hai::varray<particle> particles{100};
sitime::stopwatch watch{};

export void emit(particle p) {
  p.started = watch.millis();
  particles.push_back(p);
}

export [[nodiscard]] bool draw() {
  auto ms = watch.millis();
  bool animating{};

  for (auto i = 0; i < particles.size(); i++) {
    auto &p = particles[i];
    if (p.started + p.timeout < ms) {
      p = particles.pop_back();
      i--;
      continue;
    }

    auto &s = p.sprite;
    float dt = ms - p.started;
    s.rotation = dt;
    s.alpha = 1.0 - dotz::min(dt / p.timeout, 1.0);

    qsu::blit(s);
    animating = true;
  }

  return animating;
}
} // namespace party
