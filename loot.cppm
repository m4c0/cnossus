export module loot;
import dotz;
import hai;
import lootroll;
import map;
import qsu;
import sitime;
import spr;

static constexpr const auto anim_dur_ms = 100.0f;

export namespace loot {
struct loot {
  dotz::ivec2 coord{};
  dotz::ivec2 old_coord{};
  spr::id spr{spr::nil};
  bool visited{};

  sitime::stopwatch anim{};
};

hai::array<loot> list{map::height};

void init(int level) {
  for (auto y = 2; y < map::height - 2; y++) {
    auto x = map::pick_empty_space(y);
    if (x == -1) {
      list[y] = {};
      continue;
    }

    list[y] = loot{
        .coord = {x, y},
        .old_coord = {x, y},
        .spr = lootroll(level),
    };
  }
}

void draw(dotz::vec2 center, int rad) {
  for (auto &e : list) {
    auto f = e.anim.millis() / anim_dur_ms;
    if (f > 1.0) {
      f = 1.0;
      e.old_coord = e.coord;
    }
    auto p = dotz::mix(e.old_coord, e.coord, f);

    float a = 1.0;
    auto d = dotz::abs(p - center) - rad;
    if (d.x > 1 || d.y > 1) {
      a = 0.0;
    } else if (d.x > 0 || d.y > 0) {
      a = 1.0 - dotz::max(d.x, d.y);
    } else {
      e.visited = true;
    }

    if (!e.visited)
      continue;

    float aa = dotz::mix(0.6, 1.0, a);
    qsu::guard::multiplier m{{1, 1, 1, aa}};
    qsu::blit(e.spr, e.coord.x, e.coord.y);
  }
}

loot *at(dotz::ivec2 p) {
  auto &l = list[p.y];
  if (l.coord.x != p.x)
    return nullptr;
  if (l.spr == spr::nil)
    return nullptr;

  return &l;
}
} // namespace loot
