module;
extern "C" int rand();

export module cno:random;

namespace cno {
[[nodiscard]] inline auto random(unsigned max) noexcept { return rand() % max; }
} // namespace cno
