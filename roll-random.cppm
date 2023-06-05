export module roll:random;
import rng;

namespace roll {
static constexpr const auto item_roll_count = 5;

[[nodiscard]] inline auto random_item_id() {
  return rng::rand(item_roll_count);
}

export template <typename Tp, unsigned N> class table {
  const Tp *m_table[N]{};

public:
  constexpr table() = default;
  constexpr table(auto... d) : m_table{d...} {}

  [[nodiscard]] auto roll() const noexcept { return m_table[rng::rand(N)]; }
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

class loot_table {
  static constexpr const auto max_elems = 256;

  unsigned m_weights[max_elems]{};
  unsigned m_sum{};

public:
  constexpr void set_weight(unsigned elem, unsigned val) noexcept {
    auto &w = m_weights[elem];

    m_sum += val;
    m_sum -= w;
    w = val;
  }
  constexpr auto weight(unsigned elem) const noexcept {
    return m_weights[elem];
  }

  unsigned pick() const noexcept {
    auto r = rng::rand(max_elems);
    for (auto i = 0; i < max_elems; i++) {
      auto w = m_weights[i];
      if (w < r)
        return i;
      r -= w;
    }
    // This should never happen
    return max_elems - 1;
  }
};
} // namespace roll
