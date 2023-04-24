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

// Missing in previous code
// constexpr const auto cloth =
//   item_type{"linen tunic"}.defense(1).inventory_at(1, 3);
// constexpr const auto leather =
//   item_type{"leather vest"}.defense(2).inventory_at(1, 4);
// constexpr const auto sword =
//   item_type{"short sword"}.attack(12);

constexpr const auto bag = item_type{"bag", 1};

constexpr const auto knife = item_type{"rust knife"}.attack(2);
constexpr const auto sickle = item_type{"sickle"}.attack(3);
constexpr const auto adze = item_type{"adze"}.attack(5);
constexpr const auto axe = item_type{"axe"}.attack(8);
constexpr const auto doubleaxe = item_type{"battle axe"}.attack(10);
constexpr const auto spear = item_type{"spear"}.attack(15);

constexpr const auto driedfruit =
    item_type{"strange fruit"}.restores(3).inventory_at(0, 0);
constexpr const auto cheese =
    item_type{"cheese"}.restores(5).inventory_at(0, 1);
constexpr const auto rawmeat =
    item_type{"raw meat"}.restores(8).inventory_at(0, 2);

constexpr const auto torch =
    item_type{"torch"}.light_provided(10).inventory_at(0, 3);
constexpr const auto candle =
    item_type{"candle"}.light_provided(15).inventory_at(0, 4);
constexpr const auto oillamp =
    item_type{"lamp"}.light_provided(25).inventory_at(0, 5);

constexpr const auto greave =
    item_type{"boots"}.defense_pair(1).inventory_at(1, 0);
constexpr const auto armguard =
    item_type{"gloves"}.defense_pair(1).inventory_at(1, 1);
constexpr const auto pauldron =
    item_type{"helmet"}.defense(1).inventory_at(1, 2);
constexpr const auto scale =
    item_type{"chest armor"}.defense(3).inventory_at(1, 3);
constexpr const auto shield = item_type{"shield"}.defense(5).inventory_at(1, 4);

constexpr const auto jar = item_type{"jar"};     // TODO: drops
constexpr const auto chest = item_type{"chest"}; // TODO: drops

constexpr const item_type item_types[] = {
    adze,      armguard,   axe,    bag,    candle, cheese,  chest,
    doubleaxe, driedfruit, greave, jar,    knife,  oillamp, pauldron,
    rawmeat,   scale,      shield, sickle, spear,  torch,
};
} // namespace cno
