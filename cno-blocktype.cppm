export module cno:blocktype;
import jute;

namespace cno {
struct block_type {
  jute::view name{};
  char character{};
  bool can_walk{};
  bool transparent{};
};
[[nodiscard]] constexpr bool operator==(const block_type &a,
                                        const block_type &b) noexcept {
  return a.character == b.character;
}

constexpr const block_type blank{
    .name = "void",
    .character = ' ',
    .transparent = true,
};

// Varies with floor
constexpr const block_type dot{
    .name = "floor",
    .character = '.',
    .can_walk = true,
};
constexpr const block_type hash{
    .name = "wall",
    .character = '&',
};

// Fixed textures
constexpr const block_type comma{
    .name = "mosaic",
    .character = ',',
    .can_walk = true,
};
constexpr const block_type tilda{
    .name = "pool",
    .character = '%',
};

// Requires combining with "floor"
constexpr const block_type vbar{
    .name = "knowledge",
    .character = '#',
    .transparent = true,
};
constexpr const block_type star{
    .name = "fountain",
    .character = '$',
    .transparent = true,
};
constexpr const block_type gt{
    .name = "stairs",
    .character = '<',
    .can_walk = true,
    .transparent = true,
};
constexpr const block_type andsign{
    .name = "statue",
    .character = '(',
    .transparent = true,
};
} // namespace cno
