export module enemies;
import anim;
import dotz;
import lootroll;
import map;
import mobroll;
import rng;
import spr;

namespace enemies {
export unsigned max_enemies = map::height;

export struct enemy : anim::t {
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
        anim::t {
            .coord = {x, y},
            .anim_coord = {x, y},
            .spr = spr,
        },
        life_of(spr),
    };
  }
}

export void draw() {
  for (auto &e : d.list) {
    anim::blit(e, true);
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

  auto d = p - e.coord;
  d.x = d.x > 1 ? 1 : d.x < -1 ? -1 : d.x;
  d.y = d.y > 1 ? 1 : d.y < -1 ? -1 : d.y;

  if (is_aggressive(e.spr)) {
    if (lights_on) {
      return random_move();
    } else {
      return chase_player(d);
    }
  } else {
    if (lights_on) {
      return chase_player(d) * -1;
    } else {
      return random_move();
    }
  }
}
} // namespace enemies

module :private;
enemies::data enemies::d{};
