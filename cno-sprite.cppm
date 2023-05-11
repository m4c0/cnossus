export module cno:sprite;
import :globals;
import quack;
import traits;

namespace cno {
enum svis { sv_none, sv_fog, sv_visible };

class sid {
  char m_value;

public:
  constexpr sid(char v) : m_value{v} {}

  [[nodiscard]] constexpr operator char() const noexcept { return m_value; }

  [[nodiscard]] constexpr bool operator==(const sid &o) const noexcept {
    return m_value == o.m_value;
  }

  [[nodiscard]] constexpr quack::uv uv() const noexcept {
    auto u = static_cast<float>(m_value % 16);
    auto v = static_cast<float>(m_value / 16);

    auto u0 = u / 16.0f;
    auto v0 = v / 16.0f;
    auto u1 = (u + 1.0f) / 16.0f;
    auto v1 = (v + 1.0f) / 16.0f;
    return quack::uv{{u0, v0}, {u1, v1}};
  }
};

template <typename Tp>
concept sid_holder = requires(const Tp &x) {
  { x.id } -> traits::same_as<const sid &>;
};

[[nodiscard]] constexpr bool operator==(const sid_holder auto &a,
                                        const sid_holder auto &b) noexcept {
  return a.id == b.id;
}

template <sid_holder Tp> class stype {
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

  [[nodiscard]] constexpr bool operator==(const stype<Tp> &o) const noexcept {
    return *m_type == *o.m_type;
  }
};

template <sid_holder Tp> struct sprite {
  stype<Tp> type{};
  map_coord coord{};
  svis vis{};
};

template <typename Tp, unsigned Max>
class sbatch : public quack::instance_layout<Tp, Max> {
  using parent_t = quack::instance_layout<Tp, Max>;

  void resize(unsigned w, unsigned h) override {
    this->batch()->resize(map_width, map_height, w, h);
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

  bool find_at(map_coord c, auto &&fn) {
    for (auto &m : this->data()) {
      if (m.type && m.coord == c) {
        fn(m);
        return true;
      }
    }
    return false;
  }

  void update_rogueview(map_coord pc, unsigned radius) noexcept {
    for (auto &blk : this->data()) {
      if (!blk.type) {
        blk.vis = sv_none;
        continue;
      }

      auto dx = pc.x - blk.coord.x;
      auto dy = pc.y - blk.coord.y;
      if (dx * dx + dy * dy <= radius * radius) {
        blk.vis = sv_visible;
      } else if (blk.vis == sv_visible) {
        blk.vis = sv_fog;
      }
    }
  }

  void fill_quack() noexcept {
    this->fill_colour([](const auto &i) { return quack::colour{}; });
    this->fill_mult([](const auto &i) {
      switch (i.vis) {
      case sv_visible:
        return quack::colour{1, 1, 1, 1};
      case sv_none:
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
