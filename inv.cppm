export module inv;
import quack;
import spr;

namespace inv {
struct data {
  bool bag{};
  spr::id lights[3]{};
  spr::id food[3]{};
  spr::id weapon{};
  spr::id armour[5]{};
} d;

// TODO: weapons
// TODO: armour

export void init(int level) {
  if (level == 1) {
    d = {};
  }
}

auto blit(spr::id id, float &x, float y, quack::mapped_buffers &all) {
  if (id == spr::nil)
    return 0U;
  return spr::blit(id, x++, y, all);
}
export auto draw(quack::mapped_buffers &all) {
  constexpr const auto y = 1;
  auto count = 0;
  float x = 0;

  if (d.bag)
    count += inv::blit(spr::bag, x, y, all);

  for (auto l : d.lights)
    count += inv::blit(l, x, y, all);

  for (auto f : d.food)
    count += inv::blit(f, x, y, all);

  return count;
}

bool take(spr::id item, spr::id (&list)[3]) {
  if (!d.bag && list[0] != spr::nil) {
    return false;
  }
  for (auto &l : list) {
    if (l != spr::nil)
      continue;
    l = item;
    return true;
  }
  return false;
}
export bool take(spr::id item) {
  switch (item) {
  case spr::bag:
    if (d.bag)
      return false;

    d.bag = true;
    return true;

  case spr::torch:
  case spr::candle:
  case spr::oillamp:
    return take(item, d.lights);

  case spr::driedfruit:
  case spr::cheese:
  case spr::rawmeat:
    return take(item, d.food);

  default:
    return false;
  }
}
} // namespace inv
