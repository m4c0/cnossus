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

  x = 0;
  count += inv::blit(d.weapon, x, y + 1, all);

  for (auto a : d.armour)
    count += inv::blit(a, x, y + 1, all);

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

static auto weapon_attack(spr::id item) {
  switch (item) {
  case spr::knife:
    return 2;
  case spr::sickle:
    return 3;
  case spr::adze:
    return 5;
  case spr::axe:
    return 8;
  case spr::doubleaxe:
    return 10;
  case spr::sword:
    return 11;
  case spr::spear:
    return 15;
  default:
    return 1;
  }
}
static bool take_weapon(spr::id item) {
  auto att = weapon_attack(item);
  if (att <= weapon_attack(d.weapon))
    return false;

  d.weapon = item;
  return true;
}

static bool take_armour(spr::id item, spr::id &i) {
  if (i != spr::nil)
    return false;

  i = item;
  return true;
}

export bool take(spr::id item) {
  switch (item) {
  case spr::bag:
    if (d.bag)
      return false;

    d.bag = true;
    return true;

  case spr::knife:
  case spr::sickle:
  case spr::adze:
  case spr::axe:
  case spr::doubleaxe:
  case spr::sword:
  case spr::spear:
    return take_weapon(item);

  case spr::greave:
    return take_armour(item, d.armour[0]);
  case spr::armguard:
    return take_armour(item, d.armour[1]);
  case spr::pauldron:
    return take_armour(item, d.armour[2]);
  case spr::scale:
    return take_armour(item, d.armour[3]);
  case spr::shield:
    return take_armour(item, d.armour[4]);

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

static int defense_of(spr::id item) {
  switch (item) {
  case spr::greave:
  case spr::armguard:
  case spr::pauldron:
    return 1;
  case spr::scale:
    return 3;
  case spr::shield:
    return 5;
  default:
    return 0;
  }
}
export int defense() {
  int res{};
  for (auto a : d.armour)
    res += defense_of(a);
  return res;
}
export int attack() { return weapon_attack(d.weapon); }
} // namespace inv
