export module sfx;

import plush;

namespace sfx {
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
      .main_volume = 0.5,
      .wave_fn = plush::sqr::vol_at,
  });
}
} // namespace sfx
