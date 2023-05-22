export module qsu:layout;
import :coord;
import :id;
import :vis;
import ecs;
import quack;

export namespace qsu {
class layout : public quack::instance_layout<void, ecs::max_entities> {
  using parent_t = quack::instance_layout<void, ecs::max_entities>;

  void resize(unsigned w, unsigned h) override { batch()->resize(5, 5, w, h); }

public:
  using parent_t::parent_t;

  void center_view(coord pc) noexcept {
    batch()->center_at(pc.x + 0.5f, pc.y + 0.5f);
  }

  void fill_quack(ecs::ec *ec) noexcept {
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

template <typename Tp, unsigned W, unsigned H>
class layout2 : public quack::instance_layout<Tp, W * H> {
  using parent_t = quack::instance_layout<Tp, W * H>;

  void resize(unsigned w, unsigned h) override {
    this->batch()->resize(5, 5, w, h);
  }

public:
  using parent_t::parent_t;

  void add(Tp n) {
    for (auto &i : this->data()) {
      if (i.type)
        continue;

      i = n;
      break;
    }
  }

  void for_each(auto &&fn) {
    for (auto &m : this->data()) {
      if (m.type)
        fn(m);
    }
  }

  bool find_at(coord c, auto &&fn) {
    for (auto &m : this->data()) {
      if (m.type && m.coord == c) {
        fn(m);
        return true;
      }
    }
    return false;
  }

  void center_view(coord pc) noexcept {
    this->batch()->center_at(pc.x + 0.5f, pc.y + 0.5f);
  }

  void update_rogueview(coord pc, unsigned radius) noexcept {
    center_view(pc);

    for (auto &blk : this->data()) {
      if (!blk.type) {
        blk.vis = v_none;
        continue;
      }

      auto dx = pc.x - blk.coord.x;
      auto dy = pc.y - blk.coord.y;
      if (dx * dx + dy * dy <= radius * radius) {
        blk.vis = v_visible;
      } else if (blk.vis == v_visible) {
        blk.vis = v_fog;
      }
    }
  }

  void fill_quack() noexcept {
    this->fill_colour([](const auto &i) { return quack::colour{}; });
    this->fill_mult([](const auto &i) {
      switch (i.vis) {
      case v_visible:
        return quack::colour{1, 1, 1, 1};
      case v_none:
        return quack::colour{1, 1, 1, 0};
      default:
        return quack::colour{1, 1, 1, 0.8f};
      }
    });
    this->fill_pos([](const auto &i) {
      const auto &c = i.coord;
      auto x = static_cast<float>(c.x);
      auto y = static_cast<float>(c.y);
      return quack::rect{x, y, 1, 1};
    });
    this->fill_uv([](const auto &i) -> quack::uv {
      return i.type ? i.type->id.uv() : quack::uv{};
    });
  }
};
} // namespace qsu
