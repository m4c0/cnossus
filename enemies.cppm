export module enemies;
import dotz;
import hai;
import lootroll;
import map;
import mobroll;
import qsu;
import rng;
import spr;

export namespace enemies {
struct enemy {
  dotz::ivec2 coord{};
  spr::id spr{spr::nil};
  int life{};
};

hai::array<enemy> list{map::height};

auto life_of(spr::id id) {
  switch (id) {
  case spr::scorpion:
  case spr::spider:
    return 4;
  case spr::rat:
  case spr::centipede:
    return 6;
  case spr::boar:
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

void draw() {
  for (auto &e : list) {
    qsu::blit(e.spr, e.coord.x, e.coord.y);
  }
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

  // TODO: move depending on light and behaviour
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

unsigned poison_of(spr::id e) {
  switch (e) {
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

  if (roll <= 0)
    return;

  e.life -= rng::rand(roll);
  if (e.life <= 0)
    e = {
        .coord = e.coord,
        .spr = lootroll(e.spr),
    };
}
} // namespace enemies
