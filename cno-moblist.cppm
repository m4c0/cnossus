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

  sprite<mob_type> type{};
  map_coord coord{};
  ranged life{type ? type->life : 0};
  ranged actions{initial_max_actions};
  unsigned poison{};
  float damage_timer{};
  bonus bonus{};
};
[[nodiscard]] constexpr auto is_player(const mob &a) noexcept {
  return a.type->hostility == h_none;
}

class mob_list : public sbatch<mob, max_mobs_per_level> {
public:
  using sbatch::sbatch;

  void fill_quack(map_coord pc, unsigned d) {
    sbatch::fill_quack(pc, d);

    const auto &[px, py] = pc;
    fill_colour([px, py, d](auto &i) {
      if (!i.type)
        return quack::colour{};

      const auto &[x, y] = i.coord;
      auto dx = px - x;
      auto dy = py - y;

      const auto &[r, b] = i.type->id.uv().start;
      auto a = (dx * dx + dy * dy) <= d ? 1.0f : 0.3f;
      return quack::colour{r, 0, b, a};
    });
  }
};
} // namespace cno
