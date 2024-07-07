export module enemies;
import dotz;
import hai;
import lootroll;
import map;
import mobroll;
import party;
import qsu;
import rng;
import spr;
import timeline;

namespace enemies {
export struct enemy : qsu::anim {
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
  for (auto &e : list) {
    qsu::blit(e, center, rad);
  }
}

export void move(enemy &e, dotz::ivec2 p) {
  tim::add({
      .pos{
          .target = &e.anim_coord,
          .a = e.coord,
          .b = p,
      },
      .start = tim::anim_dur_ms,
      .length = tim::anim_dur_ms,
  });
  e.anim_coord = e.coord;
  e.coord = p;
}
export void attack(enemy &e, dotz::ivec2 p) {
  tim::add({
      .pos{
          .target = &e.anim_coord,
          .a = e.coord,
          .b = p,
          .func = tim::fn::half_and_back,
      },
      .start = tim::anim_dur_ms,
      .length = tim::anim_dur_ms,
  });
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

export void hit(enemy &e, int roll) {
  if (e.spr == spr::nil)
    return;

  // TODO: animate
  if (roll <= 0)
    return;

  e.life -= rng::rand(roll);
  if (e.life <= 0) {
    party::emit({
        .sprite{
            .id = e.spr,
            .pos = e.coord,
        },
        .timeout = 500,
    });
    e = {{
        .coord = e.coord,
        .spr = lootroll(e.spr),
    }};
  } else {
    tim::add({
        .rot{
            .target = &e.rotation,
            .a = 0,
            .b = rng::rand(2) ? -100.f : 100.f,
            .func = tim::fn::half_and_back,
        },
        .length = tim::anim_dur_ms,
    });
  }
}
} // namespace enemies
