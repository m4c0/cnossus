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
export auto draw(quack::mapped_buffers &all) {
  //
  return 0;
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
