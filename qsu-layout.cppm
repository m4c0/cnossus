export module qsu:layout;
import :id;
import ecs;
import silog;
import sires;
import quack;

export namespace qsu {
class layout : public quack::ilayout {
public:
  explicit layout(quack::renderer *r) : ilayout{r, ecs::max_entities} {
    (*this)->set_grid(7, 7);
  }

  void load_atlas() {
    // TODO: use the same trick as FOD
    constexpr const auto sz = 512;
    (*this)->load_atlas(sz, sz, [](auto *p) {
      sires::open("atlas.img")
          .fmap([p](auto &rdr) { return rdr->read(p, sz * sz * 4); })
          .map([] { silog::log(silog::info, "Atlas loaded"); })
          .take([](auto err) { silog::log(silog::error, "%s", err); });
    });
  }

  void center_view(float x, float y) noexcept {
    (*this)->center_at(x + 0.5f, y + 0.5f);
  }

  void fill_quack(ecs::ec *ec) noexcept {
    ec->sprites.sort([](const auto &a, const auto &b) noexcept {
      return a.layer - b.layer;
    });
    (*this)->map_colours([ec](auto *cs) {
      for (auto _ : ec->sprites) {
        *cs++ = {};
      }
    });
    (*this)->map_multipliers([ec](auto *ms) {
      for (auto [spr, _] : ec->sprites) {
        *ms++ = {1, 1, 1, spr.alpha};
      }
    });
    (*this)->map_positions([ec](auto *ps) {
      for (auto [spr, id] : ec->sprites) {
        auto [x, y] = ec->coords.get(id);
        *ps++ = quack::rect{static_cast<float>(x), static_cast<float>(y), 1, 1};
      }
    });
    (*this)->map_uvs([ec](auto *uvs) {
      for (auto [spr, _] : ec->sprites) {
        *uvs++ = id{spr.id}.uv();
      }
    });
    (*this)->set_count(ec->sprites.size());
  }
};
} // namespace qsu
