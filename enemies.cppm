export module enemies;
import dotz;
import hai;
import lootroll;
import map;
import mobroll;
import quack;
import rng;
import spr;

static auto life_of(spr::id id) {
  switch (id) {
  case spr::scorpion:
  case spr::spider:
    return 4;
  case spr::rat:
  case spr::centipede:
    return 6;
  case spr::cerberus:
  case spr::harpy:
  case spr::snake:
    return 8;
  case spr::bull:
  case spr::chimera:
  case spr::griffin:
  case spr::manticore:
  case spr::sphinx:
    return 12;
  case spr::crocodile:
  case spr::drakon:
    return 20;
  default:
    return 0;
  }
}

export namespace enemies {
struct enemy {
  dotz::ivec2 coord{};
  spr::id spr{spr::nil};
  int life{};
};

hai::array<enemy> list{map::height};

void init(int level) {
  for (auto y = 2; y < map::height - 2; y++) {
    auto x = map::pick_empty_space(y);
    if (x == -1) {
      list[y] = {};
      continue;
    }

    auto spr = mobroll(level);
    list[y] = enemy{
        .coord = {x, y},
        .spr = spr,
        .life = life_of(spr),
    };
  }
}

auto draw(quack::mapped_buffers &all) {
  unsigned count{};
  for (auto &e : list) {
    count += spr::blit(e.spr, e.coord.x, e.coord.y, all);
  }
  return count;
}

enemy *at(dotz::ivec2 p) {
  for (auto &e : list) {
    if (e.spr == spr::nil)
      continue;
    if (e.coord == p)
      return &e;
  }
  return nullptr;
}

dotz::ivec2 next_move(const enemy &e) {
  if (e.life <= 0)
    return {};

  switch (e.spr) {
  case spr::nil:
    return {};
  default:
    switch (rng::rand(4)) {
    case 0:
      return {-1, 0};
    case 1:
      return {1, 0};
    case 2:
      return {0, -1};
    case 3:
      return {0, 1};
    default:
      return {};
    }
  }
}

unsigned poison_of(const enemy &e) {
  switch (e.spr) {
  case spr::snake:
  case spr::spider:
    return 4;
  case spr::centipede:
  case spr::scorpion:
    return 8;
  case spr::chimera:
    return 10;
  default:
    return 0;
  }
}

void hit(enemy &e, int roll) {
  if (e.spr == spr::nil)
    return;

  // TODO: defense roll
  // auto def = life_of(e.spr);

  e.life -= roll;
  if (e.life <= 0)
    e = {
        .coord = e.coord,
        .spr = lootroll(e.spr),
    };
}
} // namespace enemies
