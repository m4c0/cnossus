export module ecs:compos;

namespace ecs::c {
struct enemy {};
struct exit {};
struct player {};
struct usable {};

struct sprite {
  char id;
  float alpha;
  bool seen;
};
} // namespace ecs::c
