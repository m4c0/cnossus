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
} // namespace qsu

namespace qsu::guard {
template <typename T, T &Target> class guard : no::no {
  T m_prev;

public:
  guard(T m) {
    m_prev = Target;
    Target = m;
  }
  ~guard() { Target = m_prev; }
};

export using multiplier = guard<quack::colour, qsu::multiplier>;
export using position = guard<dotz::vec2, qsu::pos>;
} // namespace qsu::guard
