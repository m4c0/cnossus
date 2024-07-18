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

  auto value = static_cast<float>(charge) / max_charge;
  qsu::draw_bar(value, spr::torch, x, {1, 1, 0, 1});
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
