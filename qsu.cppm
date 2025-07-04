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

export void draw(quack::instance *& i, auto &&fn) {
  current_instance = i;
  fn();
  i = current_instance;
}

} // namespace qsu

namespace qsu::guard {
export class colour : no::no {
  dotz::vec4 m_prev = g_colour;

public:
  colour(dotz::vec4 c) { g_colour = c; }
  ~colour() { g_colour = m_prev; }
};
export class multiplier : no::no {
  dotz::vec4 m_prev = qsu::multiplier;

public:
  multiplier(dotz::vec4 c) { qsu::multiplier = c; }
  ~multiplier() { qsu::multiplier = m_prev; }
};
export class position : no::no {
  dotz::vec2 m_prev = qsu::pos;

public:
  position(dotz::vec2 c) { qsu::pos = c; }
  ~position() { qsu::pos = m_prev; }
};
} // namespace qsu::guard

namespace qsu {
export struct anim {
  dotz::ivec2 coord{};
  dotz::vec2 anim_coord{};
  ::spr::id spr{::spr::nil};
  float rotation{};
  bool visited{};
};

export void blit(anim &e, dotz::vec2 center, int rad, float min = 0.0) {
  auto ad = dotz::abs(e.anim_coord - center);
  auto d = ad - rad;
  float a = 1.0;
  float max = 1.0;
  if (d.x > 3 || d.y > 3) {
    return;
  } else if (e.visited && (ad.x > 3 || ad.y > 3)) {
    max = min;
    min = 0.0;
    ad = ad - 2;
    a = 1.0 - dotz::max(ad.x, ad.y);
  } else if (d.x > 1 || d.y > 1) {
    a = 0.0;
  } else if (d.x > 0 || d.y > 0) {
    a = 1.0 - dotz::max(d.x, d.y);
  } else {
    e.visited = true;
  }
  if (!e.visited)
    min = 0.0;

  auto aa = dotz::mix(min, max, a);
  if (aa > 0.0) {
    qsu::guard::multiplier dim{{1.0f, 1.0f, 1.0f, aa}};
    qsu::blit(e.spr, e.anim_coord, e.rotation);
  }
}
} // namespace qsu
