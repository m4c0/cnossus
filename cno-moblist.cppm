export module cno:moblist;
import :mobtype;
import quack;

namespace cno {
class ranged {
  unsigned m_value;
  unsigned m_max;

public:
  constexpr explicit ranged(unsigned max) : m_value{max}, m_max{max} {}

  constexpr operator unsigned() const noexcept { return m_value; }

  constexpr auto &operator+=(unsigned delta) noexcept {
    auto n = m_value + delta;
    m_value = n > m_max ? m_max : n;
    return *this;
  }
  constexpr auto &operator-=(unsigned delta) noexcept {
    m_value = delta > m_value ? 0 : m_value - delta;
    return *this;
  }
  constexpr auto &operator--() noexcept { return *this -= 1; }

  constexpr void add_max(int delta) noexcept { m_max += delta; }
  constexpr void to_max() noexcept { m_value = m_max; }
};

struct bonus {
  int attack;
  int defense;
  int damage;
};
struct mob {
  static constexpr const auto initial_max_actions = 20;

  const mob_type *type{};
  map_coord coord{};
  ranged life{type == nullptr ? 0 : type->life};
  ranged actions{initial_max_actions};
  unsigned poison{};
  float damage_timer{};
  bonus bonus{};
};
[[nodiscard]] constexpr auto is_player(const mob &a) noexcept {
  return a.type->hostility == h_none;
}

class mob_list : quack::instance_layout<mob, max_mobs_per_level> {
  void resize(unsigned w, unsigned h) override {
    batch()->resize(map_width, map_height, w, h);
  }

public:
  explicit mob_list(quack::renderer *r) : instance_layout{r} {}

  using instance_layout::at;
  using instance_layout::process_event;

  void fill_quack(map_coord pc, unsigned d) {
    const auto &[px, py] = pc;

    fill_pos([](auto &i) {
      if (i.type == nullptr)
        return quack::rect{};

      const auto &c = i.coord;
      return quack::rect{static_cast<float>(c.x), static_cast<float>(c.y), 1,
                         1};
    });
    fill_colour([px, py, d](auto &i) {
      if (i.type == nullptr)
        return quack::colour{};

      const auto &[x, y] = i.coord;
      auto dx = px - x;
      auto dy = py - y;

      const auto &[r, b] = i.type->id.uv().start;
      auto a = (dx * dx + dy * dy) <= d ? 1.0f : 0.3f;
      return quack::colour{r, 0, b, a};
    });
    fill_uv([](auto &i) {
      return (i.type == nullptr) ? quack::uv{} : i.type->id.uv();
    });
    fill_mult([px, py, d](auto &i) {
      const auto &[x, y] = i.coord;
      auto dx = x - px;
      auto dy = y - py;
      auto a = ((dx * dx) + (dy * dy) <= d * d) ? 1.0f : 0.3f;
      return quack::colour{1, 1, 1, a};
    });
  }

  [[nodiscard]] constexpr mob *mob_at(map_coord c) {
    for (auto &m : data()) {
      if (m.type != nullptr && m.coord == c)
        return &m;
    }
    return nullptr;
  }

  void for_each(auto &&fn) {
    for (auto &m : data()) {
      if (m.type != nullptr)
        fn(m);
    }
  }
};
} // namespace cno
