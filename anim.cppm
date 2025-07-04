export module anim;
import dotz;
import spr;
import qsu;

namespace anim {
  export struct t {
    dotz::ivec2 coord{};
    dotz::vec2 anim_coord{};
    ::spr::id spr{::spr::nil};
    float rotation{};
    bool visited{};
  };

  export dotz::vec2 fog_center;
  export int fog_radius;

  static void blit(t &e, float min) {
    auto ad = dotz::abs(e.anim_coord - fog_center);
    auto d = ad - fog_radius;
    float a = 1.0;
    float max = 1.0;
    if (d.x > 3 || d.y > 3) {
      return;
    } else if (e.visited && (ad.x > 3 || ad.y > 3)) {
      max = min;
      min = 0.0;
      ad = ad - 2;
      a = 1.0 - dotz::max(ad.x, ad.y);
    } else if (d.x > 1 || d.y > 1) {
      a = 0.0;
    } else if (d.x > 0 || d.y > 0) {
      a = 1.0 - dotz::max(d.x, d.y);
    } else {
      e.visited = true;
    }
    if (!e.visited)
      min = 0.0;

    auto aa = dotz::mix(min, max, a);
    if (aa > 0.0) {
      qsu::blit({
        .id = e.spr,
        .pos = e.anim_coord,
        .alpha = aa,
        .rotation = e.rotation,
      });
    }
  }
  export void blit(t &e, bool hide_in_fog) {
    blit(e, hide_in_fog ? 0.0f : 0.6f);
  }
}
