export module cno:random;
import rng;

namespace cno {
static constexpr const auto max_level = 20;

[[nodiscard]] int roll_dice(int dice, int dice_count) noexcept {
  auto roll = 0;
  for (auto i = 0; i < dice_count; i++) {
    roll += 1 + rng::rand(dice);
  }
  return roll;
}
} // namespace cno
