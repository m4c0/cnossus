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

export spr::id lootroll(int level, spr::id origin) {
  switch (origin) {
  case spr::jar:
    return jar_loot(level);
  case spr::coffer:
    return spr::nil;
  default:
    return spr::nil;
  }
}
