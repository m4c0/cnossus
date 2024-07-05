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

    p.rotation = (ms - p.started);

    qsu::blit(p.spr, p.pos, p.rotation);
    animating = true;
  }

  return animating;
}
} // namespace party
