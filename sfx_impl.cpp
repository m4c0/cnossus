module sfx;
import plush;

float sfx::main_volume = sfx::max_volume;

void sfx::menu_move() {
  plush::play({
      .adsr{
          .decay_time = 0.1,
          .sustain_level = 0.5,
          .release_time = 0.2,
      },
      .freq{
          .start_freq = 100,
          .slide = -5,
      },
      .main_volume = main_volume,
      .wave_fn = plush::saw::vol_at,
  });
}

void sfx::menu_select() {
  plush::play({
      .adsr{
          .decay_time = 0.1,
          .sustain_level = 0.5,
          .release_time = 0.2,
      },
      .freq{
          .start_freq = 100,
          .slide = -5,
      },
      .main_volume = main_volume,
      .wave_fn = plush::saw::vol_at,
  });
}
