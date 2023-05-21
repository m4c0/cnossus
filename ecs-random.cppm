export module ecs:random;

extern "C" int rand();

namespace ecs {
[[nodiscard]] inline unsigned random(unsigned limit) { return rand() % limit; }
} // namespace ecs
