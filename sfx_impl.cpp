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
      .vib{
          .depth = 0.1,
          .speed = 1.3,
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
          .start_freq = 80,
          .slide = -10,
          .delta_slide = 2000,
      },
      .vib{
          .depth = 0.5,
          .speed = 1.3,
      },
      .main_volume = main_volume,
      .wave_fn = plush::saw::vol_at,
  });
}

void sfx::fail() {
  plush::play({
      .adsr{
          .sustain_level = 1.0,
          .release_time = 0.2,
      },
      .freq{
          .start_freq = 80,
          .delta_slide = -2000,
      },
      .vib{
          .depth = 0.5,
          .speed = 1.3,
      },
      .main_volume = main_volume,
      .wave_fn = plush::saw::vol_at,
  });
}

void sfx::eat() {}
void sfx::light() {}

void sfx::pick() {
  plush::play({
      .adsr{
          .decay_time = 0.1,
          .sustain_level = 1.3,
          .release_time = 0.2,
      },
      .freq{
          .start_freq = 900,
      },
      .arp{
          .limit = 0.05,
          .mod = 0.9,
      },
      .main_volume = sfx::main_volume * 0.8f,
      .wave_fn = plush::sqr::vol_at,
  });
}

void sfx::walk() {}

void sfx::break_jar() {
  plush::play({
      .adsr{
          .sustain_level = 1.0,
          .release_time = 0.2,
      },
      .freq{
          .start_freq = 400,
          .delta_slide = -3000,
      },
      .main_volume = sfx::main_volume,
      .wave_fn = plush::noise::vol_at,
  });
}
