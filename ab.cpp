#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fstream>
#include <iostream>

constexpr const auto tile_side = 32;
constexpr const auto tile_stride = 16;
constexpr const auto atlas_width = tile_side * tile_stride;

struct rgb {
  int r;
  int g;
  int b;
  int a;
};

rgb pixies[atlas_width][atlas_width]{};

bool stamp(const char *fname, char id) {
  int w;
  int h;
  int n;
  auto data = stbi_load(fname, &w, &h, &n, 4);
  if (data == nullptr) {
    std::cerr << "Failure reading image"
              << "\n";
    return false;
  }

  if (w != h && w != tile_side) {
    std::cerr << "Expecting tiles to be a square of " << tile_side << "x"
              << tile_side << "\n";
    return false;
  }

  for (auto y = 0; y < tile_side; y++) {
    auto &p_row = pixies[y];
    auto *t_row = &data[y * tile_side * 4];
    for (auto x = 0; x < tile_side; x++) {
      auto *t_pix = &t_row[x * 4];
      p_row[x] = {
          .r = t_pix[0],
          .g = t_pix[1],
          .b = t_pix[2],
          .a = t_pix[3],
      };
    }
  }

  return true;
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

  if (!stamp("atlas/adze.png", '0'))
    return 1;

  f << "P3\n" << atlas_width << " " << atlas_width << " 255\n";
  for (auto y = 0; y < atlas_width; y++) {
    for (auto x = 0; x < atlas_width; x++) {
      auto &c = pixies[y][x];
      f << c.r << " " << c.g << " " << c.b << "\n";
    }
  }

  return 0;
}
