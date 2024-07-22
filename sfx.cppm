export module sfx;

import plush;

namespace sfx {
export constexpr const auto max_volume = 0.5f;
export extern float main_volume;
export void menu_move() {
  plush::play({
      .adsr{
          .decay_time = 0.1,
          .sustain_level = 1.3,
          .release_time = 0.2,
      },
      .freq{
          .start_freq = 30000,
      },
      .main_volume = main_volume,
      .wave_fn = plush::sqr::vol_at,
  });
}
} // namespace sfx

module :private;

float sfx::main_volume = 0.5;
