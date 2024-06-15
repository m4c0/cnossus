export module qsu;
import dotz;
import no;
import spr;
import quack;

namespace qsu {
export using colour = quack::colour;

quack::mapped_buffers *current_buffers{};
unsigned count{};
quack::colour multiplier{1, 1, 1, 1};

// TODO: global for multiplier, etc
export void blit(spr::id i, float x, float y) {
  if (i == spr::nil)
    return;

  auto uv = dotz::vec2{i % 16, i / 16} / 16.0;

  auto &[c, m, p, u] = *current_buffers;
  *c++ = {};
  *m++ = multiplier;
  *p++ = {{x, y}, {1, 1}};
  *u++ = {uv, uv + 1.0 / 16.0};
  count++;
}

export auto draw(quack::mapped_buffers all, auto &&fn) {
  count = 0;
  current_buffers = &all;
  fn();
  return count;
}
} // namespace qsu

export namespace qsu::guard {
class multiplier : no::no {
  quack::colour m_prev;

public:
  multiplier(quack::colour m) {
    m_prev = qsu::multiplier;
    qsu::multiplier = m;
  }
  ~multiplier() { qsu::multiplier = m_prev; }
};
} // namespace qsu::guard
