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

template <unsigned MaxElems> class random_picker {
  static constexpr const auto max_elems = MaxElems;

  unsigned m_weights[max_elems]{};
  unsigned m_sum{};

public:
  [[nodiscard]] constexpr auto operator[](unsigned elem) noexcept {
    class it {
      unsigned *m_w;
      unsigned *m_sum;

    public:
      explicit constexpr it(unsigned *w, unsigned *s) : m_w{w}, m_sum{s} {}

      it &operator=(unsigned w) {
        *m_sum += w;
        *m_sum -= w;
        *m_w = w;
        return *this;
      }
    };
    return it{&m_weights[elem], &m_sum};
  }

  [[nodiscard]] unsigned pick() const noexcept {
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
template <typename Tp, unsigned MaxElements> class loot_table {
  random_picker<MaxElements> m_picker{};
  Tp m_elements[MaxElements]{};

public:
  explicit loot_table(auto... is) : m_elements{is...} {}

  [[nodiscard]] constexpr auto operator[](unsigned elem) noexcept {
    return m_picker[elem];
  }

  [[nodiscard]] Tp pick() const noexcept { return m_elements[m_picker.pick()]; }
  void pick(auto fn) const noexcept {
    Tp p = pick();
    if (p != nullptr)
      fn(p);
  }
};
} // namespace roll
