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
};

hai::varray<particle> particles{100};
sitime::stopwatch watch{};

export void emit(particle p) {
  p.timeout += watch.millis();
  particles.push_back(p);
}

static void cleanup(int ms) {
  for (auto i = 0; i < particles.size(); i++) {
    auto &p = particles[i];
    if (p.timeout < ms)
      continue;

    p = particles.pop_back();
    i--;
  }
}
export void tick() {
  auto ms = watch.millis();

  for (auto &p : particles) {
  }

  cleanup(ms);
}
} // namespace party
