export module cno:entities;
import :objects;
import :random;
import jute;
import qsu;

namespace cno {
struct item_type {
  jute::view name;
  qsu::id id;
  int attack{};
  int defense{};
  unsigned life_gain{};
  int light_provided{};
};

enum hostilities { h_none, h_scaried, h_aggresive };

struct mob_type {
  jute::view name;
  qsu::id id;
  unsigned life;
  unsigned dice = life;
  hostilities hostility;
  unsigned poison{};
};

struct bonus {
  int attack;
  int defense;
  int damage;
};
struct mob {
  static constexpr const auto initial_max_actions = 20;

  const mob_type *type{};
  ranged life{type ? type->life : 0};
  ranged actions{initial_max_actions};
  unsigned poison{};
  float damage_timer{};
  bonus bonus{};
};
[[nodiscard]] constexpr auto is_player(const mob &a) noexcept {
  return a.type->hostility == h_none;
}
} // namespace cno
