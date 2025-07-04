#pragma leco add_resource "atlas.png"

export module qsu;
import dotz;
import jute;
import no;
import spr;
import quack;

namespace qsu {
export struct sprite {
  spr::id id;
  dotz::vec2 pos;
  dotz::vec2 size{1};
  dotz::vec4 colour{};
  float alpha{1};
  float rotation{};
};
export struct grid {
  dotz::vec2 grid_pos;
  dotz::vec2 grid_size;
};

quack::instance *current_instance{};
dotz::vec4 multiplier{1, 1, 1, 1};
dotz::vec4 g_colour{};
dotz::vec2 pos{};

export void init() {
  using namespace quack::donald;

  app_name("cnossus");
  atlas("atlas.png");
  clear_colour({ 0, 0, 0, 1 });
  max_quads(10240);
}

export void grid_size(grid g) {
  quack::donald::push_constants({
    .grid_pos = g.grid_pos,
    .grid_size = g.grid_size,
  });
}

export dotz::vec2 mouse_pos() {
  return quack::donald::mouse_pos();
}

export void screenshot() { quack::donald::offscreen(800, 600); }

export void blit(const sprite &s) {
  if (s.id == spr::nil)
    return;

  auto uv = dotz::vec2{s.id % 16, s.id / 16} / 16.0;
  *current_instance++ = quack::instance{
      .position = s.pos + pos,
      .size = s.size,
      .uv0 = uv,
      .uv1 = uv + 1.0 / 16.0,
      .colour = s.colour,
      .multiplier{1.f, 1.f, 1.f, s.alpha},
      .rotation{s.rotation, 0.5, 0.5},
  };
}
export void blit(spr::id i, float x, float y, float rot) {
  blit({
      .id = i,
      .pos{x, y},
      .alpha = multiplier.w,
      .rotation = rot,
  });
}
export inline void blit(spr::id i, dotz::vec2 p, float rot) {
  blit(i, p.x, p.y, rot);
}

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

export void draw(auto && fn) {
  quack::donald::data([=](quack::instance *& all) {
    current_instance = all;
    fn();
    all = current_instance;
  });
}

} // namespace qsu

namespace qsu::guard {
export class position : no::no {
  dotz::vec2 m_prev = qsu::pos;

public:
  position(dotz::vec2 c) { qsu::pos = c; }
  ~position() { qsu::pos = m_prev; }
};
} // namespace qsu::guard

