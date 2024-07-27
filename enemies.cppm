export module enemies;
import dotz;
import lootroll;
import map;
import mobroll;
import qsu;
import rng;
import spr;

namespace enemies {
export unsigned max_enemies = map::height;

export struct enemy : qsu::anim {
  int life{};
};

export extern struct data {
  enemy list[map::height];
} d;

export void init(int level) {
  for (auto y = 2; y < map::height - 2; y++) {
    auto x = map::pick_empty_space(y);
    if (x == -1) {
      d.list[y] = {};
      continue;
    }

    auto spr = mobroll(level);
    d.list[y] = enemy{
        qsu::anim{
            .coord = {x, y},
            .anim_coord = {x, y},
            .spr = spr,
        },
        life_of(spr),
    };
  }
}

export void draw(dotz::vec2 center, int rad) {
  for (auto &e : d.list) {
    qsu::blit(e, center, rad);
  }
}

export enemy *at(dotz::ivec2 p) {
  for (auto &e : d.list) {
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
static dotz::ivec2 chase_player(const dotz::ivec2 d) {
  if (d.x == 0 || d.y == 0)
    return d;

  switch (rng::rand(2)) {
  case 0:
    return {d.x, 0};
  case 1:
    return {0, d.y};
  default:
    return {};
  }
}
export dotz::ivec2 next_move(const enemy &e, const dotz::ivec2 p,
                             bool lights_on) {
  if (e.life <= 0 || e.spr == spr::nil)
    return {};

  if (is_aggressive(e.spr)) {
    if (lights_on) {
      return random_move();
    } else {
      return chase_player(p - e.coord);
    }
  } else {
    if (lights_on) {
      return chase_player(p - e.coord) * -1;
    } else {
      return random_move();
    }
  }
}
} // namespace enemies

module :private;
enemies::data enemies::d{};
