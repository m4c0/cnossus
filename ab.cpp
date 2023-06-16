import spr;
import stubby;

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#include <fstream>
#include <iostream>

constexpr const auto tile_side = 32;
constexpr const auto tile_stride = 16;
constexpr const auto atlas_width = tile_side * tile_stride;
constexpr const auto num_channels = 4;

struct rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

rgb pixies[atlas_width][atlas_width]{};

void stamp(const char *fname, char id) {
  auto img = stbi::load(fname);
  if (*img.data == nullptr) {
    std::cerr << "Failure reading " << fname << "\n";
    throw 0;
  }

  uint8_t data[tile_side * tile_side * num_channels];
  stbir_resize_uint8(*img.data, img.width, img.height, 0, data, tile_side,
                     tile_side, 0, num_channels);

  unsigned tx = (id % tile_stride) * tile_side;
  unsigned ty = (id / tile_stride) * tile_side;
  for (auto y = 0; y < tile_side; y++) {
    auto &p_row = pixies[y + ty];
    auto *t_row = &data[y * tile_side * num_channels];
    for (auto x = 0; x < tile_side; x++) {
      auto *t_pix = &t_row[x * num_channels];
      p_row[tx + x] = {
          .r = t_pix[0],
          .g = t_pix[1],
          .b = t_pix[2],
          .a = t_pix[3],
      };
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <output-ppm>\n";
    return 1;
  }
  std::ofstream f{argv[1]};
  if (!f) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 1;
  }

  try {
    stamp("atlas/floor.png", spr::floor);
    stamp("atlas/wall.png", spr::wall);
    stamp("atlas/mosaic.png", spr::mosaic);
    stamp("atlas/pool.png", spr::pool);
    stamp("atlas/column.png", spr::column);
    stamp("atlas/basin.png", spr::basin);
    stamp("atlas/stairs.png", spr::exit);
    stamp("atlas/statue.png", spr::statue);

    // stamp("atlas/tunic", spr::cloth);
    // stamp("atlas/vest", spr::leather);
    // stamp("atlas/sword", spr::sword);
    stamp("atlas/bag.png", spr::bag);
    stamp("atlas/knife.png", spr::knife);
    stamp("atlas/sickle.png", spr::sickle);
    stamp("atlas/adze.png", spr::adze);
    stamp("atlas/axe.png", spr::axe);
    stamp("atlas/doubleaxe.png", spr::doubleaxe);
    stamp("atlas/spear.png", spr::spear);
    stamp("atlas/driedfruit.png", spr::driedfruit);
    stamp("atlas/cheese.png", spr::cheese);
    stamp("atlas/rawmeat.png", spr::rawmeat);
    stamp("atlas/torch.png", spr::torch);
    stamp("atlas/candle.png", spr::candle);
    stamp("atlas/oillamp.png", spr::oillamp);
    stamp("atlas/greave.png", spr::greave);
    stamp("atlas/armguard.png", spr::armguard);
    stamp("atlas/pauldron.png", spr::pauldron);
    stamp("atlas/scale.png", spr::scale);
    stamp("atlas/shield.png", spr::shield);
    stamp("atlas/jar.png", spr::jar);
    stamp("atlas/coffer.png", spr::coffer);

    stamp("atlas/minotaur.png", spr::minotaur);
    stamp("atlas/snake.png", spr::snake);
    stamp("atlas/spider.png", spr::spider);
    stamp("atlas/rat.png", spr::rat);
    stamp("atlas/scorpion.png", spr::scorpion);
    stamp("atlas/centipede.png", spr::centipede);
    stamp("atlas/cerberus.png", spr::cerberus);
    stamp("atlas/harpy.png", spr::harpy);
    stamp("atlas/chimera.png", spr::chimera);
    stamp("atlas/manticore.png", spr::manticore);
    stamp("atlas/crocodile.png", spr::crocodile);
    stamp("atlas/drakon.png", spr::drakon);
    stamp("atlas/boar.png", spr::boar);
    stamp("atlas/griffin.png", spr::griffin);
    stamp("atlas/sphinx.png", spr::sphinx);
    stamp("atlas/bull.png", spr::bull);
  } catch (...) {
    return 1;
  }

  f.rdbuf()->sputn(reinterpret_cast<const char *>(pixies),
                   atlas_width * atlas_width * num_channels);
  return 0;
}
