module;
extern "C" int rand();

export module cno:random;
import :globals;

namespace cno {
[[nodiscard]] inline auto random(unsigned max) noexcept { return rand() % max; }

[[nodiscard]] int roll_dice(int dice, int dice_count) noexcept {
  auto roll = 0;
  for (auto i = 0; i < dice_count; i++) {
    roll += 1 + cno::random(dice);
  }
  return roll;
}

template <typename Tp, unsigned N> class rnd_roll {
  Tp m_table[N]{};

public:
  constexpr rnd_roll() = default;
  constexpr rnd_roll(auto... d) : m_table{d...} {}

  [[nodiscard]] auto roll() const noexcept { return m_table[random(N)]; }
};

template <typename Tp, unsigned N> class rnd_roll_per_level {
  // Level 21 is game over, so nothing should be there
  rnd_roll<Tp, N> m_table[max_level + 1]{};

public:
  using roll_t = rnd_roll<Tp, N>;

  constexpr rnd_roll_per_level() = default;
  constexpr rnd_roll_per_level(auto... d) : m_table{d...} {}

  [[nodiscard]] auto roll(unsigned lvl) const noexcept {
    return m_table[lvl - 1].roll();
  }
};
} // namespace cno

static_assert([] {
  cno::rnd_roll_per_level<int, 5> s{
      cno::rnd_roll<int, 5>{0, 1, 2},
      cno::rnd_roll<int, 5>{1},
      cno::rnd_roll<int, 5>{2, 3, 4, 5},
  };
  return true;
}());
