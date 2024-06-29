#pragma leco add_resource "atlas.png"

export module qsu;
import dotz;
import no;
import spr;
import quack;
import voo;

namespace qsu {
export using colour = quack::colour;

quack::mapped_buffers *current_buffers{};
unsigned count{};
quack::colour multiplier{1, 1, 1, 1};
quack::colour g_colour{};
dotz::vec2 pos{};

export void blit(spr::id i, float x, float y) {
  if (i == spr::nil)
    return;

  auto uv = dotz::vec2{i % 16, i / 16} / 16.0;
  quack::pos pp{pos.x + x, pos.y + y};

  auto &[c, m, p, u] = *current_buffers;
  *c++ = g_colour;
  *m++ = multiplier;
  *p++ = {pp, {1, 1}};
  *u++ = {uv, uv + 1.0 / 16.0};
  count++;
}
export inline void blit(spr::id i, dotz::vec2 p) { blit(i, p.x, p.y); }

export auto draw(quack::mapped_buffers all, auto &&fn) {
  count = 0;
  current_buffers = &all;
  fn();
  return count;
}

export quack::donald::atlas_t *atlas(voo::device_and_queue *dq) {
  return new voo::sires_image("atlas.png", dq);
}

} // namespace qsu

namespace qsu::guard {
export class colour : no::no {
  quack::colour m_prev = g_colour;

public:
  colour(quack::colour c) { g_colour = c; }
  ~colour() { g_colour = m_prev; }
};
export class multiplier : no::no {
  quack::colour m_prev = qsu::multiplier;

public:
  multiplier(quack::colour c) { qsu::multiplier = c; }
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
export struct spr {
  dotz::ivec2 coord{};
  dotz::vec2 anim_coord{};
  ::spr::id spr{::spr::nil};
  bool visited{};
};

export void blit(spr &e, dotz::vec2 center, int rad, float min = 0.0) {
  auto d = dotz::abs(e.anim_coord - center) - rad;
  if (d.x <= 1 && d.y <= 1) {
    e.visited = true;
  }
  if (!e.visited)
    return;

  float a = 1.0;
  if (d.x > 1 || d.y > 1) {
    a = 0.0;
  } else if (d.x > 0 || d.y > 0) {
    a = 1.0 - dotz::max(d.x, d.y);
  }
  auto aa = dotz::mix(min, 1.0, a);

  qsu::guard::multiplier dim{{1, 1, 1, aa}};
  qsu::blit(e.spr, e.anim_coord);
}
} // namespace qsu
