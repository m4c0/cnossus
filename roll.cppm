export module roll;
export import :itemdrop;
export import :itemlevel;
export import :random;
import rng;

namespace roll {
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
