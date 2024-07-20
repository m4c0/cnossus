export module light;
import qsu;
import rng;
import spr;

namespace light {
static constexpr const auto max_charge = 25;

export extern struct data {
  int charge{};
} d;

export void init(int lvl) {
  if (lvl == 1)
    d = {};
}

export void draw_ui() {
  constexpr const auto x = 3.5;

  auto value = static_cast<float>(d.charge) / max_charge;
  qsu::draw_bar(value, spr::torch, x, {1, 1, 0, 1});
}

export void tick() {
  if (d.charge <= 0)
    return;

  if (rng::rand(3) == 0)
    d.charge--;
}

export void restore(int roll) {
  d.charge += roll;

  if (d.charge >= max_charge)
    d.charge = max_charge;
}
} // namespace light
module :private;
light::data light::d{};
