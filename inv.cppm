export module inv;
import quack;
import spr;

namespace inv {
bool bag{};
spr::id lights[3]{};

export void init(int level) {
  if (level == 1) {
    bag = false;
    for (auto &l : lights)
      l = spr::nil;
  }
}

auto blit(spr::id id, float &x, float y, quack::mapped_buffers &all) {
  if (id == spr::nil)
    return 0U;
  return spr::blit(id, x++, y, all);
}
export auto draw(quack::mapped_buffers &all) {
  auto count = 0;
  float x = 0;

  if (bag)
    count += spr::blit(spr::bag, x, 0, all);

  for (auto l : lights)
    count += inv::blit(l, x, 0, all);

  return count;
}

export bool take(spr::id item) {
  switch (item) {
  case spr::bag:
    if (bag)
      return false;

    bag = true;
    return true;
  case spr::torch:
  case spr::candle:
  case spr::oillamp:
    if (!bag && lights[0] != spr::nil) {
      return false;
    }
    for (auto &l : lights) {
      if (l != spr::nil)
        continue;
      l = item;
      return true;
    }
    return false;
  default:
    return false;
  }
}
} // namespace inv
