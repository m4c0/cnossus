export module ecs:mobroll;
import :ec;
import :loot;
import :mobs;
import pog;

namespace ecs {
auto add_easy_non_hostile_mob(ec *ec) {
  return loot_table<2>{&add_spider, &add_rat}.pick()(ec);
}
auto add_medium_non_hostile_mob(ec *ec) {
  return loot_table<3>{&add_snake, &add_scorpion, &add_centipede}.pick()(ec);
}

auto add_easy_hostile_mob(ec *ec) {
  return loot_table<3>{&add_harpy, &add_boar, &add_cerberus}.pick()(ec);
}
auto add_medium_hostile_mob(ec *ec) {
  return loot_table<4>{&add_manticore, &add_griffin, &add_sphinx, &add_bull}
      .pick()(ec);
}
auto add_hard_hostile_mob(ec *ec) {
  return loot_table<3>{&add_chimera, &add_croc, &add_drakon}.pick()(ec);
}

export auto add_level_mob(ec *ec, unsigned lvl) {
  auto lt =
      loot_table<6>{&add_easy_non_hostile_mob, &add_medium_non_hostile_mob,
                    &add_easy_hostile_mob,     &add_medium_hostile_mob,
                    &add_hard_hostile_mob,     nullptr};
  lt[0] = lt[5] = 20;

  if (lvl > 2) { // EH appears
    lt[2] = 20;
  }
  if (lvl > 5) { // MNH appears
    lt[1] = 20;
  }
  if (lvl > 8) { // MH appears
    lt[3] = 20;
  }
  if (lvl > 10) { // Mob count increases
    lt[5] = 1;
  }
  if (lvl > 12) { // No easy mobs
    lt[0] = lt[2] = 0;
  }
  if (lvl > 15) { // HH appears
    lt[4] = 20;
  }

  auto p = lt.pick();
  return (p == nullptr) ? pog::eid{} : p(ec);
}

static constexpr const auto mobs_per_level = map_height - 2;
export void add_level_mobs(ec *ec, unsigned lvl) {
  for (auto i = 0; i < mobs_per_level; i++) {
    add_level_mob(ec, lvl);
  }
}
} // namespace ecs
