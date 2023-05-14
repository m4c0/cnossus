#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fstream>
#include <iostream>

constexpr const auto tile_side = 32;
constexpr const auto tile_stride = 16;
constexpr const auto atlas_width = tile_side * tile_stride;

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

  return 0;
}
