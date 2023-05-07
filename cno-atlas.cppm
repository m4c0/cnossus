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

static constexpr const pixel floor_pal_0{16, 32, 64, 255};
static constexpr const pixel floor_pal_1{64, 96, 128, 255};
static constexpr const pixel wall_pal_0{128, 140, 160, 255};
static constexpr const pixel wall_pal_1{140, 150, 160, 255};
static constexpr const pixel water_pal_0{0, 64, 196, 255};
static constexpr const pixel water_pal_1{16, 96, 196, 255};

static constexpr all_chars glyphs = [] {
  all_chars res{};

  res.chars[comma.character] = {{
      {floor_pal_0, floor_pal_1, floor_pal_0, floor_pal_1},
      {floor_pal_1, floor_pal_0, floor_pal_1, floor_pal_0},
      {floor_pal_0, floor_pal_1, floor_pal_0, floor_pal_1},
      {floor_pal_1, floor_pal_0, floor_pal_1, floor_pal_0},
  }};
  res.chars[dot.character] = {{
      {floor_pal_0, floor_pal_0, floor_pal_0, floor_pal_0},
      {floor_pal_0, floor_pal_0, floor_pal_0, floor_pal_0},
      {floor_pal_0, floor_pal_0, floor_pal_0, floor_pal_0},
      {floor_pal_0, floor_pal_0, floor_pal_0, floor_pal_0},
  }};
  res.chars[gt.character] = {{
      {floor_pal_0, floor_pal_0, floor_pal_0, wall_pal_1},
      {floor_pal_0, floor_pal_0, wall_pal_0, wall_pal_0},
      {floor_pal_0, wall_pal_1, wall_pal_1, wall_pal_0},
      {wall_pal_0, wall_pal_0, wall_pal_0, wall_pal_0},
  }};
  res.chars[hash.character] = {{
      {wall_pal_0, wall_pal_1, wall_pal_1, wall_pal_1},
      {wall_pal_0, wall_pal_0, wall_pal_0, wall_pal_0},
      {wall_pal_1, wall_pal_1, wall_pal_1, wall_pal_0},
      {wall_pal_0, wall_pal_0, wall_pal_0, wall_pal_0},
  }};
  res.chars[tilda.character] = {{
      {water_pal_0, water_pal_1, water_pal_1, water_pal_1},
      {water_pal_0, water_pal_0, water_pal_0, water_pal_0},
      {water_pal_1, water_pal_1, water_pal_1, water_pal_0},
      {water_pal_0, water_pal_0, water_pal_0, water_pal_0},
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
