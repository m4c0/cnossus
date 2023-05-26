export module rng;

extern "C" int rand();

namespace rng {
export class randomizer {
  unsigned x = 0x69696969;

public:
  [[nodiscard]] constexpr auto random() {
    if consteval {
      /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
      x ^= x << 13;
      x ^= x >> 17;
      x ^= x << 5;
      return x;
    } else {
      return static_cast<unsigned>(rand());
    }
  }
  [[nodiscard]] constexpr auto random(unsigned max) { return random() % max; }
};

export [[nodiscard]] inline auto random(unsigned max) noexcept {
  return rand() % max;
}

} // namespace rng
