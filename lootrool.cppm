export module lootrool;
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

export spr::id lootrool(int level) {
  if (level <= 14) {
    return initial_item();
  } else if (level <= 17) {
    return midgame_item();
  } else {
    return endgame_item();
  }
}
