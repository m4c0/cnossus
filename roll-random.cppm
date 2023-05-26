export module roll:random;
import rng;

export namespace roll {
template <typename Tp, unsigned N> class table {
  const Tp *m_table[N]{};

public:
  constexpr table() = default;
  constexpr table(auto... d) : m_table{d...} {}

  [[nodiscard]] auto roll() const noexcept { return m_table[rng::random(N)]; }
};

template <typename Tp, unsigned N, unsigned Levels> class level_table {
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
