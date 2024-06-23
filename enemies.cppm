export module enemies;
import dotz;
import hai;
import lootroll;
import map;
import mobroll;
import qsu;
import rng;
import spr;

namespace enemies {
export struct enemy {
  dotz::ivec2 coord{};
  spr::id spr{spr::nil};
  int life{};
};

export hai::array<enemy> list{map::height};

export void init(int level) {
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

export void draw(dotz::ivec2 center, int rad) {
  for (auto &e : list) {
    auto [dx, dy] = dotz::abs(e.coord - center);
    if (dx > rad || dy > rad) {
      continue;
    }
    qsu::blit(e.spr, e.coord.x, e.coord.y);
  }
}

export enemy *at(dotz::ivec2 p) {
  for (auto &e : list) {
    if (e.spr == spr::nil)
      continue;
    if (e.coord == p)
      return &e;
  }
  return nullptr;
}

static dotz::ivec2 random_move() {
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
export dotz::ivec2 next_move(const enemy &e, bool lights_on) {
  if (e.life <= 0)
    return {};

  // TODO: move depending on light and behaviour
  switch (e.spr) {
  case spr::nil:
    return {};
  default:
    return random_move();
  }
}

export void hit(enemy &e, int roll) {
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
