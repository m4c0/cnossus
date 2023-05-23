export module ecs:compos;

namespace ecs::c {
struct marker {};

struct sprite {
  char id;
  float alpha;
  bool seen;
};
} // namespace ecs::c
