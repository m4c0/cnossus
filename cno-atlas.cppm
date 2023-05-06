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

class image {
  constexpr void load(const auto &t, const char_pixels &c_data, pixel *a_data) {
    auto c = t.character();
    auto cx = glyph_width * (c % 16);
    auto cy = glyph_height * (c / 16);
    auto da_base = &a_data[cy * width + cx];
    for (auto y = 0; y < glyph_height; y++) {
      auto da_row = &da_base[y * width];
      auto &dc_row = c_data.data[y];
      for (auto x = 0; x < glyph_width; x++) {
        da_row[x] = dc_row[x];
      }
    }
  }

  static constexpr const pixel black{0, 0, 0, 255};
  static constexpr const pixel white{255, 255, 255, 255};

  static constexpr char_pixels b_dot{{
      {black, white, black, white},
      {white, black, white, black},
      {black, white, black, white},
      {white, black, white, black},
  }};

public:
  void operator()(pixel *data) { load(dot, image::b_dot, data); }
};

void load(quack::renderer &r) { r.load_atlas(width, height, image{}); }
} // namespace cno::atlas
