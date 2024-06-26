export module enemies;
import dotz;
import hai;
import lootroll;
import map;
import mobroll;
import qsu;
import rng;
import sitime;
import spr;

namespace enemies {
static constexpr const auto anim_dur_ms = 100.0f;

export struct enemy {
  dotz::ivec2 coord{};
  dotz::ivec2 old_coord{};
  spr::id spr{spr::nil};
  int life{};

  sitime::stopwatch anim{};
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
        .old_coord = {x, y},
        .spr = spr,
        .life = life_of(spr),
    };
  }
}

export void draw(dotz::vec2 center, int rad) {
  for (auto &e : list) {
    float a = 1.0;
    auto d = dotz::abs(e.coord - center) - rad;
    if (d.x > 1 || d.y > 1) {
      a = 0.5;
    } else if (d.x > 0 || d.y > 0) {
      a = (1.0 - dotz::max(d.x, d.y)) * 0.5 + 0.5;
    }

    auto f = e.anim.millis() / anim_dur_ms;
    if (f > 1.0) {
      f = 1.0;
      e.old_coord = e.coord;
    }
    auto p = dotz::mix(e.old_coord, e.coord, f);

    qsu::guard::multiplier m{{1, 1, 1, a}};
    qsu::blit(e.spr, p.x, p.y);
  }
}

export void move(enemy &e, dotz::ivec2 p) {
  e.old_coord = e.coord;
  e.coord = p;
  e.anim = {};
}

export bool is_animating() {
  for (auto &e : list)
    if (e.old_coord != e.coord)
      return true;

  return false;
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
