export module ui;
import dotz;
import jute;
import qsu;
import spr;

namespace ui {
  export void draw_bar(float value, spr::id icon, float x, dotz::vec4 colour) {
    constexpr const auto y = 3.5f;
    constexpr const auto bar_t = -y;
    constexpr const auto bar_b = y;
    constexpr const auto full_bar_h = bar_b - bar_t;

    auto bar_h = full_bar_h * value;
    auto bar_y = bar_t + full_bar_h * (1.0f - value);

    // TODO: animate to final position (like Street Fighter)
    qsu::blit(qsu::sprite{
      .id = spr::cursor,
      .pos = dotz::vec2{x, bar_y} + 0.05,
      .size = dotz::vec2{1.f, bar_h} - 0.1,
      .colour = colour,
    });

    qsu::blit(icon, x, y, 0);
    qsu::blit(spr::ui_bar_b, x, y - 1, 0);
    for (auto yy = y - 2; yy > -y; yy--) {
      qsu::blit(spr::ui_bar, x, yy, 0);
    }
    qsu::blit(spr::ui_bar_t, x, -y, 0);
  }

  export void draw_str(jute::view str, float x, float y, float a = 1.0) {
    for (unsigned c : str) {
      qsu::blit(qsu::sprite{
        .id = static_cast<spr::id>(c + 128),
        .pos{x++, y},
        .alpha = a,
      });
    }
  }
}
