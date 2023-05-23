export module map:block;

export namespace map {
struct block {
  char id;
  bool can_walk{};
};

constexpr const block blank{
    .id = ' ',
};

// Varies with floor
constexpr const block dot{
    .id = '.',
    .can_walk = true,
};
constexpr const block hash{
    .id = '&',
};

// Fixed textures
constexpr const block comma{
    .id = ',',
    .can_walk = true,
};
constexpr const block tilda{
    .id = '%',
};

// Requires combining with "floor"
constexpr const block vbar{
    .id = '#',
};
constexpr const block star{
    .id = '$',
};
constexpr const block gt{
    .id = '<',
    .can_walk = true,
};
constexpr const block andsign{
    .id = '(',
};
} // namespace map
