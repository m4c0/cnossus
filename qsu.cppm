export module qsu;
import dotz;
import spr;
import quack;

namespace qsu {
export quack::mapped_buffers *current_buffers{};
export unsigned count{};

// TODO: global for multiplier, etc
export void blit(spr::id i, float x, float y) {
  if (i == spr::nil)
    return;

  auto uv = dotz::vec2{i % 16, i / 16} / 16.0;

  auto &[c, m, p, u] = *current_buffers;
  *c++ = {};
  *m++ = {1, 1, 1, 1};
  *p++ = {{x, y}, {1, 1}};
  *u++ = {uv, uv + 1.0 / 16.0};
  count++;
}
} // namespace qsu
