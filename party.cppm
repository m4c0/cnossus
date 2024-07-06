export module party;

import dotz;
import hai;
import qsu;
import sitime;
import spr;

namespace party {
struct particle {
  spr::id spr;
  dotz::vec2 pos;
  float alpha;
  float rotation;
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

    float dt = ms - p.started;
    p.rotation = dt;
    p.alpha = 1.0 - dotz::min(dt / p.timeout, 1.0);

    qsu::guard::multiplier dim{{1.0f, 1.0f, 1.0f, p.alpha}};
    qsu::blit(p.spr, p.pos, p.rotation);
    animating = true;
  }

  return animating;
}
} // namespace party
