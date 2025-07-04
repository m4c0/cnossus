export module party;

import anim;
import dotz;
import hai;
import sitime;
import spr;

namespace party {
struct particle {
  anim::t sprite;
  float timeout;
  float started;
};

hai::varray<particle> particles{100};
sitime::stopwatch watch{};

export void emit(particle p) {
  p.started = watch.millis();
  particles.push_back(p);
}

export void draw() {
  auto ms = watch.millis();

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

    anim::blit(s, true);
  }
}

export [[nodiscard]] bool is_animating() { return particles.size() > 0; }
} // namespace party
