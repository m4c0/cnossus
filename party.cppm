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

export void draw() {
  auto ms = watch.millis();

  for (auto i = 0; i < particles.size(); i++) {
    auto &p = particles[i];
    if (p.started + p.timeout < ms) {
      p = particles.pop_back();
      i--;
      continue;
    }

    qsu::blit(p.spr, p.pos, p.rotation);
  }
}
} // namespace party
