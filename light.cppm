export module light;
import qsu;
import rng;
import spr;

namespace light {
static constexpr const auto max_charge = 25;

export int charge{};

export void init(int lvl) {
  if (lvl == 1)
    charge = 0;
}

export void draw_ui() {
  constexpr const auto x = 3.5;
  constexpr const auto y = 3.5;

  for (auto i = 0; i < charge; i++) {
    qsu::blit(spr::torch, x, y - i * 8.5 / max_charge);
  }
}

export void tick() {
  if (charge <= 0)
    return;

  if (rng::rand(3) == 0)
    charge--;
}

export void restore(int roll) {
  charge += roll;

  if (charge >= max_charge)
    charge = max_charge;
}
} // namespace light
