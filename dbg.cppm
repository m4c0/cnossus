export module dbg;
import ecs;
import pog;

extern "C" int printf(const char *, ...);

namespace dbg {
export void show_all_sprites(ecs::ec *ec, float alpha = 1.0) {
  for (auto &[spr, _] : ec->sprites) {
    spr.alpha = alpha;
  }
}

template <typename Tp>
void dump_set(const char *name, const pog::singleton<Tp> &set) {
  printf("%s", name);
  auto id = set.get_id();
  if (id)
    printf(": %d", static_cast<unsigned>(id));
  printf("\n");
}

template <typename Tp>
void dump_set(const char *name, const pog::sparse_set<Tp> &set) {
  printf("%s:", name);
  for (auto [_, id] : set) {
    printf(" %d", static_cast<unsigned>(id));
  }
  printf("\n");
}

export void dump_ecs(ecs::ec *ec) {
  dump_set("armour", ec->armour);
  dump_set("bags", ec->bags);
  // dump_set("blockers",ec->blockers);
  dump_set("coords", ec->coords);
  dump_set("enemies", ec->enemies);
  dump_set("exit", ec->exit);
  dump_set("foods", ec->foods);
  dump_set("hostiles", ec->hostiles);
  dump_set("in_use", ec->in_use);
  dump_set("lights", ec->lights);
  dump_set("mobs", ec->mobs);
  dump_set("non_hostiles", ec->non_hostiles);
  dump_set("player", ec->player);
  dump_set("sprites", ec->sprites);
  dump_set("usables", ec->usables);
  dump_set("walls", ec->walls);
  dump_set("weapons", ec->weapons);
}
} // namespace dbg
