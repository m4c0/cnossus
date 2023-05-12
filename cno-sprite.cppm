export module cno:sprite;
import qsu;
import quack;
import traits;

namespace cno {
using scoord = qsu::coord;
using svis = qsu::vis;

template <qsu::id_holder Tp> class stype {
  const Tp *m_type{};

public:
  constexpr stype() noexcept = default;
  explicit constexpr stype(const Tp *t) noexcept : m_type{t} {}

  [[nodiscard]] explicit constexpr operator bool() const noexcept {
    return m_type != nullptr;
  }
  [[nodiscard]] constexpr const Tp *operator->() const noexcept {
    return m_type;
  }
};

template <qsu::id_holder Tp> struct sprite {
  stype<Tp> type{};
  scoord coord{};
  svis vis{};
};

template <typename Tp, unsigned W, unsigned H>
class sbatch : public quack::instance_layout<Tp, W * H> {
  using parent_t = quack::instance_layout<Tp, W * H>;

  void resize(unsigned w, unsigned h) override {
    this->batch()->resize(W, H, w, h);
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

  bool find_at(scoord c, auto &&fn) {
    for (auto &m : this->data()) {
      if (m.type && m.coord == c) {
        fn(m);
        return true;
      }
    }
    return false;
  }

  void update_rogueview(scoord pc, unsigned radius) noexcept {
    for (auto &blk : this->data()) {
      if (!blk.type) {
        blk.vis = qsu::v_none;
        continue;
      }

      auto dx = pc.x - blk.coord.x;
      auto dy = pc.y - blk.coord.y;
      if (dx * dx + dy * dy <= radius * radius) {
        blk.vis = qsu::v_visible;
      } else if (blk.vis == qsu::v_visible) {
        blk.vis = qsu::v_fog;
      }
    }
  }

  void fill_quack() noexcept {
    this->fill_colour([](const auto &i) { return quack::colour{}; });
    this->fill_mult([](const auto &i) {
      switch (i.vis) {
      case qsu::v_visible:
        return quack::colour{1, 1, 1, 1};
      case qsu::v_none:
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
} // namespace cno
