#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#include <fstream>
#include <iostream>

constexpr const auto tile_side = 32;
constexpr const auto tile_stride = 16;
constexpr const auto atlas_width = tile_side * tile_stride;
constexpr const auto num_channels = 4;

struct rgb {
  int r;
  int g;
  int b;
  int a;
};

rgb pixies[atlas_width][atlas_width]{};

void stamp(const char *fname, char id) {
  int w;
  int h;
  int n;
  auto raw = stbi_load(fname, &w, &h, &n, num_channels);
  if (raw == nullptr) {
    std::cerr << "Failure reading " << fname << "\n";
    throw 0;
  }

  uint8_t data[tile_side * tile_side * num_channels];
  stbir_resize_uint8(raw, w, h, 0, data, tile_side, tile_side, 0, num_channels);

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
    stamp("atlas/floor.png", '.');
    stamp("atlas/wall.png", '&');
    stamp("atlas/mosaic.png", ',');
    stamp("atlas/pool.png", '%');
    stamp("atlas/column.png", '#');
    stamp("atlas/basin.png", '$');
    stamp("atlas/stairs.png", '<');
    stamp("atlas/statue.png", '(');

    // stamp("atlas/tunic", 'a');
    // stamp("atlas/vest", 'b');
    // stamp("atlas/sword", 'c');
    stamp("atlas/bag.png", 'd');
    stamp("atlas/knife.png", 'e');
    stamp("atlas/sickle.png", 'f');
    stamp("atlas/adze.png", 'g');
    stamp("atlas/axe.png", 'h');
    stamp("atlas/doubleaxe.png", 'i');
    stamp("atlas/spear.png", 'j');
    stamp("atlas/driedfruit.png", 'k');
    stamp("atlas/cheese.png", 'l');
    stamp("atlas/rawmeat.png", 'm');
    stamp("atlas/torch.png", 'n');
    stamp("atlas/candle.png", 'o');
    stamp("atlas/oillamp.png", 'p');
    stamp("atlas/greave.png", 'q');
    stamp("atlas/armguard.png", 'r');
    stamp("atlas/pauldron.png", 's');
    stamp("atlas/scale.png", 't');
    stamp("atlas/shield.png", 'u');
    stamp("atlas/jar.png", 'v');
    stamp("atlas/coffer.png", 'w');
  } catch (...) {
    return 1;
  }

  f << "P3\n" << atlas_width << " " << atlas_width << " 255\n";
  for (auto y = 0; y < atlas_width; y++) {
    for (auto x = 0; x < atlas_width; x++) {
      auto &c = pixies[y][x];
      f << c.r << " " << c.g << " " << c.b << "\n";
    }
  }

  return 0;
}
