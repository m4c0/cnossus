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

void draw(dotz::ivec2 center, int rad) {
  for (auto &e : list) {
    auto [dx, dy] = dotz::abs(e.coord - center);
    if (dx > rad || dy > rad) {
      continue;
    }
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
