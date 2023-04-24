export module cno:itemtype;
import jute;

namespace cno {
struct inventory_pos {
  unsigned x;
  unsigned y;
};
class item_type {
  jute::view m_name;
  int m_attack{};
  int m_defense{};
  int m_max_carry{-1}; // TODO: consider a aux enum for this logic
  int m_life_gain{};
  int m_light_provided{};
  inventory_pos m_inv_coords{~0U, ~0U};
  // inventory
  // drops

public:
  explicit constexpr item_type(jute::view n) : m_name{n} {}
  explicit constexpr item_type(jute::view n, int m)
      : m_name{n}, m_max_carry{m} {}

  constexpr item_type attack(int a) const noexcept {
    auto r = *this;
    r.m_attack = a;
    r.m_max_carry = 1;
    return r;
  }
  constexpr item_type defense(int a) const noexcept {
    auto r = *this;
    r.m_defense = a;
    r.m_max_carry = 1;
    return r;
  }
  constexpr item_type defense_pair(int a) const noexcept {
    auto r = *this;
    r.m_defense = a;
    r.m_max_carry = 2;
    return r;
  }
  constexpr item_type light_provided(int a) const noexcept {
    auto r = *this;
    r.m_light_provided = a;
    return r;
  }
  constexpr item_type restores(int a) const noexcept {
    auto r = *this;
    r.m_life_gain = a;
    return r;
  }
  constexpr item_type inventory_at(unsigned x, unsigned y) const noexcept {
    auto r = *this;
    r.m_inv_coords = {x, y};
    return r;
  }
};

constexpr const item_type item_types[] = {
    item_type{"candle"}.light_provided(15).inventory_at(0, 4),
    item_type{"torch"}.light_provided(10).inventory_at(0, 3),

    item_type{"rust knife"}.attack(2),
    item_type{"sickle"}.attack(3),
    item_type{"adze"}.attack(5),
    item_type{"axe"}.attack(8),
    item_type{"battle axe"}.attack(10),
    item_type{"spear"}.attack(15),

    item_type{"raw meat"}.inventory_at(0, 2).restores(8),
    item_type{"strange fruit"}.inventory_at(0, 0).restores(3),
    item_type{"cheese"}.inventory_at(0, 1).restores(5),

    item_type{"lamp"}.inventory_at(0, 5).light_provided(25),

    item_type{"bag", 1},

    item_type{"jar"},   // TODO: drops
    item_type{"chest"}, // TODO: drops

    item_type{"boots"}.defense_pair(1).inventory_at(1, 0),
    item_type{"gloves"}.defense_pair(1).inventory_at(1, 1),
    item_type{"helmet"}.defense(1).inventory_at(1, 2),
    item_type{"chest armor"}.defense(3).inventory_at(1, 3),
    item_type{"shield"}.defense(5).inventory_at(1, 4),
};
} // namespace cno
