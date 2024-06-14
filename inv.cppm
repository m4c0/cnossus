export module inv;
import quack;
import spr;

namespace inv {
bool bag{};
spr::id lights[3]{};
spr::id food[3]{};
spr::id weapon{};
spr::id armour[5]{};

// TODO: weapons
// TODO: armour

export void init(int level) {
  if (level == 1) {
    bag = false;
    for (auto &l : lights)
      l = spr::nil;
    for (auto &f : food)
      f = spr::nil;

    weapon = spr::nil;
    for (auto &a : armour)
      a = spr::nil;
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

  if (bag)
    count += inv::blit(spr::bag, x, y, all);

  for (auto l : lights)
    count += inv::blit(l, x, y, all);

  for (auto f : food)
    count += inv::blit(f, x, y, all);

  return count;
}

bool take(spr::id item, spr::id (&list)[3]) {
  if (!bag && list[0] != spr::nil) {
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
    if (bag)
      return false;

    bag = true;
    return true;

  case spr::torch:
  case spr::candle:
  case spr::oillamp:
    return take(item, lights);

  case spr::driedfruit:
  case spr::cheese:
  case spr::rawmeat:
    return take(item, food);

  default:
    return false;
  }
}
} // namespace inv
