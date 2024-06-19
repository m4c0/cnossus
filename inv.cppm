export module inv;
import qsu;
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

void blit(spr::id id, float &x, float y) {
  if (id != spr::nil)
    qsu::blit(id, x++, y);
}
export void draw_ui() {
  constexpr const auto y = 2;
  float x = 0;

  if (d.bag)
    inv::blit(spr::bag, x, y);

  for (auto l : d.lights)
    inv::blit(l, x, y);

  for (auto f : d.food)
    inv::blit(f, x, y);

  x = 0;
  inv::blit(d.weapon, x, y + 1);

  for (auto a : d.armour)
    inv::blit(a, x, y + 1);
}

export spr::id eat_food(int id) {
  if (id < 0 || id >= 3)
    return spr::nil;
  auto res = d.food[id];
  d.food[id] = spr::nil;
  return res;
}
export spr::id burn_light(int id) {
  if (id < 0 || id >= 3)
    return spr::nil;
  auto res = d.lights[id];
  d.lights[id] = spr::nil;
  return res;
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

static bool take_weapon(spr::id item) {
  auto att = attack_of(item);
  if (att <= attack_of(d.weapon))
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

export int defense() {
  int res{};
  for (auto a : d.armour)
    res += defense_of(a);
  return res;
}
export int attack() { return attack_of(d.weapon); }
} // namespace inv
