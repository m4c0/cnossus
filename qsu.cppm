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
dotz::vec2 pos{};

export void blit(spr::id i, float x, float y) {
  if (i == spr::nil)
    return;

  auto uv = dotz::vec2{i % 16, i / 16} / 16.0;
  quack::pos pp{pos.x + x, pos.y + y};

  auto &[c, m, p, u] = *current_buffers;
  *c++ = {};
  *m++ = multiplier;
  *p++ = {pp, {1, 1}};
  *u++ = {uv, uv + 1.0 / 16.0};
  count++;
}

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
