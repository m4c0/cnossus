export module roll:random;
import rng;

namespace roll {
static constexpr const auto item_roll_count = 5;

[[nodiscard]] inline auto random_item_id() {
  return rng::random(item_roll_count);
}

export template <typename Tp, unsigned N> class table {
  const Tp *m_table[N]{};

public:
  constexpr table() = default;
  constexpr table(auto... d) : m_table{d...} {}

  [[nodiscard]] auto roll() const noexcept { return m_table[rng::random(N)]; }
};

export template <typename Tp, unsigned N, unsigned Levels> class level_table {
  table<Tp, N> m_table[Levels]{};

public:
  using roll_t = table<Tp, N>;

  constexpr level_table() = default;
  constexpr level_table(auto... d) : m_table{d...} {}

  [[nodiscard]] auto roll(unsigned lvl) const noexcept {
    return m_table[lvl - 1].roll();
  }
};
} // namespace roll
