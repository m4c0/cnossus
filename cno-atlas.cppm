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

static constexpr all_chars glyphs = [] {
  constexpr const pixel f0{16, 32, 64, 255};
  constexpr const pixel f1{64, 96, 128, 255};
  constexpr const pixel g0{128, 140, 160, 255};
  constexpr const pixel g1{140, 150, 160, 255};
  constexpr const pixel w0{0, 64, 196, 255};
  constexpr const pixel w1{16, 96, 196, 255};

  all_chars res{};

  res.chars[comma.character] = {{
      {f0, f1, f0, f1},
      {f1, f0, f1, f0},
      {f0, f1, f0, f1},
      {f1, f0, f1, f0},
  }};
  res.chars[dot.character] = {{
      {f0, f0, f0, f0},
      {f0, f0, f0, f0},
      {f0, f0, f0, f0},
      {f0, f0, f0, f0},
  }};
  res.chars[gt.character] = {{
      {f0, f0, f0, g1},
      {f0, f0, g0, g0},
      {f0, g1, g1, g0},
      {g0, g0, g0, g0},
  }};
  res.chars[hash.character] = {{
      {g0, g1, g1, g1},
      {g0, g0, g0, g0},
      {g1, g1, g1, g0},
      {g0, g0, g0, g0},
  }};
  res.chars[tilda.character] = {{
      {w0, w1, w1, w1},
      {w0, w0, w0, w0},
      {w1, w1, w1, w0},
      {w0, w0, w0, w0},
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
