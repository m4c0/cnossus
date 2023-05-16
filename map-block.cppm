export module map:block;
import jute;
import qsu;

export namespace map {
struct block {
  jute::view name;
  qsu::id id;
  bool can_walk{};
};

constexpr const block blank{
    .name = "void",
    .id = ' ',
};

// Varies with floor
constexpr const block dot{
    .name = "floor",
    .id = '.',
    .can_walk = true,
};
constexpr const block hash{
    .name = "wall",
    .id = '&',
};

// Fixed textures
constexpr const block comma{
    .name = "mosaic",
    .id = ',',
    .can_walk = true,
};
constexpr const block tilda{
    .name = "pool",
    .id = '%',
};

// Requires combining with "floor"
constexpr const block vbar{
    .name = "knowledge",
    .id = '#',
};
constexpr const block star{
    .name = "fountain",
    .id = '$',
};
constexpr const block gt{
    .name = "stairs",
    .id = '<',
    .can_walk = true,
};
constexpr const block andsign{
    .name = "statue",
    .id = '(',
};
} // namespace map
