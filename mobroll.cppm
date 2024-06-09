export module mobroll;
import rng;
import spr;

using gen_t = spr::id (*)();

spr::id easy_non_hostile_mob() {
  constexpr const auto size = 2;
  constexpr const spr::id table[size]{spr::spider, spr::rat};
  return table[rng::rand(size)];
}
spr::id medium_non_hostile_mob() {
  constexpr const auto size = 3;
  constexpr const spr::id table[size]{spr::snake, spr::scorpion,
                                      spr::centipede};
  return table[rng::rand(size)];
}
spr::id easy_hostile_mob() {
  constexpr const auto size = 3;
  constexpr const spr::id table[size]{spr::harpy, spr::boar, spr::cerberus};
  return table[rng::rand(size)];
}
spr::id medium_hostile_mob() {
  constexpr const auto size = 4;
  constexpr const spr::id table[size]{spr::manticore, spr::griffin, spr::sphinx,
                                      spr::bull};
  return table[rng::rand(size)];
}
spr::id hard_hostile_mob() {
  constexpr const auto size = 3;
  constexpr const spr::id table[size]{spr::chimera, spr::crocodile,
                                      spr::drakon};
  return table[rng::rand(size)];
}
spr::id null() { return spr::nil; }

export spr::id mobroll(int lvl) {
  constexpr const auto size = 6;
  constexpr const gen_t table[size]{
      &easy_non_hostile_mob, &medium_non_hostile_mob, &easy_hostile_mob,
      &medium_hostile_mob,   &hard_hostile_mob,       &null};

  rng::random_picker lt{size};
  lt[0] = lt[5] = 20;

  if (lvl > 2) { // EH appears
    lt[2] = 20;
  }
  if (lvl > 5) { // MNH appears
    lt[1] = 20;
  }
  if (lvl > 8) { // MH appears
    lt[3] = 20;
  }
  if (lvl > 10) { // Mob count increases
    lt[5] = 1;
  }
  if (lvl > 12) { // No easy mobs
    lt[0] = lt[2] = 0;
  }
  if (lvl > 15) { // HH appears
    lt[4] = 20;
  }

  return table[lt.pick()]();
}
