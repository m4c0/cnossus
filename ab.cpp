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
};

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

  rgb pixies[atlas_width][atlas_width]{};

  int w;
  int h;
  int n;
  auto data = stbi_load("atlas/adze.png", &w, &h, &n, 4);
  if (data == nullptr) {
    std::cerr << "Failure reading image"
              << "\n";
    return 1;
  }

  if (w != h && w != tile_side) {
    std::cerr << "Expecting tiles to be a square of " << tile_side << "x"
              << tile_side << "\n";
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
