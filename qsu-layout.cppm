export module qsu:layout;
import :id;
import ecs;
import quack;

export namespace qsu {
class layout : public quack::ilayout {
public:
  explicit layout(quack::renderer *r) : ilayout{r, ecs::max_entities} {
    set_grid(7, 7);
  }

  void center_view(float x, float y) noexcept {
    batch()->center_at(x + 0.5f, y + 0.5f);
  }

  void fill_quack(ecs::ec *ec) noexcept {
    ec->sprites.sort([](const auto &a, const auto &b) noexcept {
      return a.layer - b.layer;
    });
    batch()->colours().map([ec](auto *cs) {
      for (auto _ : ec->sprites) {
        *cs++ = {};
      }
    });
    batch()->multipliers().map([ec](auto *ms) {
      for (auto [spr, _] : ec->sprites) {
        *ms++ = {1, 1, 1, spr.alpha};
      }
    });
    batch()->positions().map([ec](auto *ps) {
      for (auto [spr, id] : ec->sprites) {
        auto [x, y] = ec->coords.get(id);
        *ps++ = quack::rect{static_cast<float>(x), static_cast<float>(y), 1, 1};
      }
    });
    batch()->uvs().map([ec](auto *uvs) {
      for (auto [spr, _] : ec->sprites) {
        *uvs++ = id{spr.id}.uv();
      }
    });
    batch()->set_count(ec->sprites.size());
  }
};
} // namespace qsu
