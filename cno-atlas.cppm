export module cno:atlas;
import :blocktype;
import :itemtype;
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
  constexpr const pixel tr{};
  constexpr const pixel f0{16, 32, 64, 255};
  constexpr const pixel f1{64, 96, 128, 255};
  constexpr const pixel g0{128, 140, 160, 255};
  constexpr const pixel g1{140, 150, 160, 255};
  constexpr const pixel w0{0, 64, 196, 255};
  constexpr const pixel w1{16, 96, 196, 255};
  constexpr const pixel br{128, 140, 70, 255};
  constexpr const pixel gr{50, 140, 10, 255};
  constexpr const pixel rd{140, 40, 10, 255};
  constexpr const pixel yl{140, 150, 10, 255};
  constexpr const pixel bl{10, 30, 140, 255};

  all_chars res{};

  res.chars[andsign.id] = {{
      {f0, g0, f0, f0},
      {f0, g0, g0, f0},
      {f0, g1, g1, f0},
      {f0, g0, g0, f0},
  }};
  res.chars[bag.id] = {{
      {tr, tr, br, tr},
      {tr, br, br, tr},
      {br, br, br, br},
      {tr, br, br, br},
  }};
  res.chars[candle.id] = {{
      {tr, rd, tr, tr},
      {tr, rd, rd, tr},
      {tr, br, br, tr},
      {tr, br, br, tr},
  }};
  res.chars[cheese.id] = {{
      {tr, tr, tr, tr},
      {yl, tr, tr, tr},
      {yl, yl, tr, tr},
      {yl, yl, yl, tr},
  }};
  res.chars[cloth.id] = {{
      {tr, gr, gr, tr},
      {gr, yl, yl, gr},
      {gr, yl, yl, gr},
      {gr, yl, yl, gr},
  }};
  res.chars[coffer.id] = {{
      {tr, tr, tr, tr},
      {tr, br, br, tr},
      {br, br, br, br},
      {br, br, br, br},
  }};
  res.chars[comma.id] = {{
      {f0, f1, f0, f1},
      {f1, f0, f1, f0},
      {f0, f1, f0, f1},
      {f1, f0, f1, f0},
  }};
  res.chars[dot.id] = {{
      {f0, f0, f0, f0},
      {f0, f0, f0, f0},
      {f0, f0, f0, f0},
      {f0, f0, f0, f0},
  }};
  res.chars[driedfruit.id] = {{
      {tr, tr, tr, tr},
      {tr, tr, tr, tr},
      {tr, yl, yl, tr},
      {tr, yl, yl, tr},
  }};
  res.chars[gt.id] = {{
      {f0, f0, f0, g1},
      {f0, f0, g0, g0},
      {f0, g1, g1, g0},
      {g0, g0, g0, g0},
  }};
  res.chars[hash.id] = {{
      {g0, g1, g1, g1},
      {g0, g0, g0, g0},
      {g1, g1, g1, g0},
      {g0, g0, g0, g0},
  }};
  res.chars[jar.id] = {{
      {tr, br, br, tr},
      {tr, br, br, tr},
      {br, br, br, br},
      {tr, br, br, tr},
  }};
  res.chars[knife.id] = {{
      {tr, tr, tr, tr},
      {tr, tr, tr, tr},
      {tr, gr, tr, tr},
      {br, tr, tr, tr},
  }};
  res.chars[star.id] = {{
      {f0, f0, w0, f0},
      {f0, w0, f0, f0},
      {g0, g0, g0, g0},
      {f0, g0, g0, f0},
  }};
  res.chars[tilda.id] = {{
      {w0, w1, w1, w1},
      {w0, w0, w0, w0},
      {w1, w1, w1, w0},
      {w0, w0, w0, w0},
  }};
  res.chars[torch.id] = {{
      {tr, tr, rd, rd},
      {tr, tr, br, rd},
      {tr, br, tr, tr},
      {br, tr, tr, tr},
  }};
  res.chars[vbar.id] = {{
      {g0, g0, g0, g1},
      {f0, g0, g0, f0},
      {f0, g1, g1, f0},
      {g0, g0, g0, g0},
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
