export module light;
import qsu;
import spr;

namespace light {
int charge{};

export void init(int lvl) {
  if (lvl == 1)
    charge = 0;
}

export void draw() {
  for (auto x = 0; x < charge; x++) {
    qsu::blit(spr::torch, x, 1);
  }
}

export void tick() {
  if (charge <= 0)
    return;

  charge--;
}
} // namespace light
