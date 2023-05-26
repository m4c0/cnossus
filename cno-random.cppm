export module cno:random;
import rng;
import roll;

namespace cno {
static constexpr const auto max_level = 20;

[[nodiscard]] int roll_dice(int dice, int dice_count) noexcept {
  auto roll = 0;
  for (auto i = 0; i < dice_count; i++) {
    roll += 1 + rng::random(dice);
  }
  return roll;
}

template <typename Tp, unsigned N> using rnd_roll = roll::table<Tp, N>;

// Level 21 is game over, so nothing should be there
template <typename Tp, unsigned N>
using rnd_roll_per_level = roll ::level_table<Tp, N, max_level + 1>;
} // namespace cno
