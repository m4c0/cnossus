export module cno:blocktype;
import :entities;

namespace cno {
constexpr const block_type blank{
    .name = "void",
    .id = ' ',
};

// Varies with floor
constexpr const block_type dot{
    .name = "floor",
    .id = '.',
    .can_walk = true,
};
constexpr const block_type hash{
    .name = "wall",
    .id = '&',
};

// Fixed textures
constexpr const block_type comma{
    .name = "mosaic",
    .id = ',',
    .can_walk = true,
};
constexpr const block_type tilda{
    .name = "pool",
    .id = '%',
};

// Requires combining with "floor"
constexpr const block_type vbar{
    .name = "knowledge",
    .id = '#',
};
constexpr const block_type star{
    .name = "fountain",
    .id = '$',
};
constexpr const block_type gt{
    .name = "stairs",
    .id = '<',
    .can_walk = true,
};
constexpr const block_type andsign{
    .name = "statue",
    .id = '(',
};
} // namespace cno
