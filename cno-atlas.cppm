export module cno:atlas;
import :blocktype;
import quack;

namespace cno::atlas {
constexpr const auto glyph_width = 4;
constexpr const auto glyph_height = 4;
constexpr const auto width = 16 * glyph_width;
constexpr const auto height = 16 * glyph_height;
using pixel = quack::u8_rgba;

struct char_pixels {
  pixel data[glyph_height][glyph_width];
};
struct all_chars {
  char_pixels chars[256];
};
struct image {
  pixel pixels[width * height];
};

static constexpr const pixel black{0, 0, 0, 255};
static constexpr const pixel white{255, 255, 255, 255};

static constexpr all_chars glyphs = [] {
  all_chars res{};

  res.chars[comma.character()] = {{
      {black, white, black, white},
      {white, black, white, black},
      {black, white, black, white},
      {white, black, white, black},
  }};
  res.chars[dot.character()] = {{
      {black, black, black, black},
      {black, black, black, black},
      {black, black, black, black},
      {black, black, black, black},
  }};
  return res;
}();
static constexpr image img = [] {
  image i{};

  for (auto c = 0; c < 256; c++) {
    auto cx = glyph_width * (c % 16);
    auto cy = glyph_height * (c / 16);
    auto da_base = &i.pixels[cy * width + cx];
    auto &dc_base = glyphs.chars[c];
    for (auto y = 0; y < glyph_height; y++) {
      auto da_row = &da_base[y * width];
      auto &dc_row = dc_base.data[y];
      for (auto x = 0; x < glyph_width; x++) {
        da_row[x] = dc_row[x];
      }
    }
  }

  return i;
}();

void load(quack::renderer &r) {
  r.load_atlas(width, height, [](auto *p) {
    for (auto i = 0; i < width * height; i++) {
      p[i] = img.pixels[i];
    }
  });
}
} // namespace cno::atlas
