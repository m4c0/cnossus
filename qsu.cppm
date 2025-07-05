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

export void blit(const sprite &s) {
  if (s.id == spr::nil)
    return;

  auto uv = dotz::vec2{s.id % 16, s.id / 16} / 16.0;
  *current_instance++ = quack::instance{
      .position = s.pos,
      .size = s.size,
      .uv0 = uv,
      .uv1 = uv + 1.0 / 16.0,
      .colour = s.colour,
      .multiplier{1.f, 1.f, 1.f, s.alpha},
      .rotation{s.rotation, 0.5, 0.5},
  };
}

export void draw(auto && fn) {
  quack::donald::data([=](quack::instance *& all) {
    current_instance = all;
    fn();
    all = current_instance;
  });
}
} // namespace qsu

