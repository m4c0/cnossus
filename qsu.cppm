#pragma leco add_resource "atlas.png"

export module qsu;
import dotz;
import no;
import spr;
import quack;
import voo;

namespace qsu {
struct sprite {
  spr::id id;
  dotz::vec2 pos;
  float alpha;
  float rotation;
};

quack::instance *current_instance{};
unsigned count{};
dotz::vec4 multiplier{1, 1, 1, 1};
dotz::vec4 g_colour{};
dotz::vec2 pos{};

export void blit(const sprite &s) {
  if (s.id == spr::nil)
    return;

  auto uv = dotz::vec2{s.id % 16, s.id / 16} / 16.0;
  *current_instance++ = quack::instance{
      .position = s.pos + pos,
      .size{1, 1},
      .uv0 = uv,
      .uv1 = uv + 1.0 / 16.0,
      .colour{},
      .multiplier{1.f, 1.f, 1.f, s.alpha},
      .rotation{s.rotation, 0.5, 0.5},
  };
  count++;
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

export auto draw(quack::instance *i, auto &&fn) {
  count = 0;
  current_instance = i;
  fn();
  return count;
}

export quack::donald::atlas_t *atlas(voo::device_and_queue *dq) {
  return new voo::sires_image("atlas.png", dq);
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
  auto d = dotz::abs(e.anim_coord - center) - rad;
  float a = 1.0;
  float max = 1.0;
  if (d.x > 3 || d.y > 3) {
    return;
  } else if (e.visited && (d.x > 2 || d.y > 2)) {
    max = min;
    min = 0.0;
    d = d - 2;
    a = 1.0 - dotz::max(d.x, d.y);
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
