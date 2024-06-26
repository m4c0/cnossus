export module lootroll;
import rng;
import spr;

static spr::id initial_item() {
  constexpr const auto count = 5;
  constexpr const spr::id items[count]{spr::jar, spr::coffer, spr::torch,
                                       spr::nil, spr::nil};
  return items[rng::rand(count)];
}
static spr::id midgame_item() {
  constexpr const auto count = 5;
  constexpr const spr::id items[count]{spr::jar, spr::coffer, spr::torch,
                                       spr::doubleaxe, spr::nil};
  return items[rng::rand(count)];
}
static spr::id endgame_item() {
  constexpr const auto count = 6;
  constexpr const spr::id items[count]{spr::jar,   spr::coffer, spr::torch,
                                       spr::spear, spr::shield, spr::nil};
  return items[rng::rand(count)];
}

export spr::id lootroll(int level) {
  if (level <= 14) {
    return initial_item();
  } else if (level <= 17) {
    return midgame_item();
  } else {
    return endgame_item();
  }
}

static spr::id jar_loot(int level) {
  constexpr const auto size = 4;
  constexpr const spr::id list[size]{spr::driedfruit, spr::candle, spr::cheese,
                                     spr::nil};
  rng::random_picker lt{size};
  lt[0] = lt[1] = 30;
  lt[2] = 30 - level;
  lt[3] = 60;
  return list[lt.pick()];
}

static spr::id coffer_loot(int level) {
  constexpr const spr::id armour[7]{
      spr::cloth,    spr::leather, spr::greave, spr::armguard,
      spr::pauldron, spr::leather, spr::scale,
  };
  constexpr const spr::id weapons[5]{
      spr::knife, spr::sickle, spr::adze, spr::axe, spr::sword,
  };
  rng::random_picker lt{5};
  lt[0] = lt[1] = lt[2] = lt[3] = 1;
  lt[4] = 2;

  const spr::id list[5]{armour[level / 3], weapons[level / 4], spr::bag,
                        spr::oillamp, spr::nil};
  return list[lt.pick()];
}

export spr::id lootroll(int level, spr::id origin) {
  switch (origin) {
  case spr::jar:
    return jar_loot(level);
  case spr::coffer:
    return coffer_loot(level);
  default:
    return spr::nil;
  }
}

static spr::id loot3(spr::id a, spr::id b = spr::nil, spr::id c = spr::nil) {
  const spr::id table[5]{a, b, c, spr::nil, spr::nil};
  return table[rng::rand(5)];
}
export spr::id lootroll(spr::id origin) {
  switch (origin) {
  case spr::snake:
    return loot3(spr::rawmeat, spr::armguard);
  case spr::rat:
    return loot3(spr::rawmeat);
  case spr::crocodile:
    return loot3(spr::rawmeat, spr::leather);
  case spr::drakon:
    return loot3(spr::shield, spr::shield);
  case spr::boar:
    return loot3(spr::rawmeat, spr::rawmeat);
  case spr::bull:
    return loot3(spr::rawmeat, spr::rawmeat, spr::rawmeat);
  default:
    return spr::nil;
  }
}
