#pragma leco tool

import plush;
import rng;
import sfx;
import sitime;

void break_or_attack_or_something() {
  plush::play({
      .adsr{
          .sustain_level = 1.0,
          .release_time = 0.1,
      },
      .freq{
          .start_freq = 200,
          .delta_slide = -2000,
      },
      .vib{
          .depth = 0.5,
          .speed = 1.3,
      },
      .main_volume = sfx::main_volume,
      .wave_fn = plush::saw::vol_at,
  });
}

void enemy_took_hit() {
  plush::play({
      .adsr{
          .sustain_level = 1.0,
          .release_time = 0.2,
      },
      .freq{
          .start_freq = 200,
          .delta_slide = -1000,
      },
      .vib{
          .depth = 0.5,
          .speed = 1.3,
      },
      .main_volume = sfx::main_volume,
      .wave_fn = plush::saw::vol_at,
  });
}

void test() {
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

int main() {
  rng::seed();

  test();

  sitime::sleep(1);
}
