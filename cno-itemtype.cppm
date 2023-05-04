export module cno:itemtype;
import :random;
import jute;

namespace cno {
static constexpr const auto max_item_drops = 5;

enum carry_type {
  carry_one,
  carry_two,
  carry_many,
};
struct inventory_pos {
  unsigned sec;
  unsigned row;
};

class item_type;
using item_loot_table = rnd_roll_per_level<const item_type *, max_item_drops>;

class item_type {
  jute::view m_name;
  char m_char;
  int m_attack{};
  int m_defense{};
  carry_type m_carry{carry_many};
  int m_life_gain{};
  int m_light_provided{};
  inventory_pos m_inv_coords{~0U, ~0U};
  const item_loot_table *m_drops{};

  explicit constexpr item_type() : m_name{""}, m_char{} {}

public:
  explicit constexpr item_type(jute::view n, char c) : m_name{n}, m_char{c} {}
  constexpr item_type(jute::view n, char c, carry_type m)
      : m_name{n}, m_char{c}, m_carry{m} {}
  constexpr item_type(jute::view n, char c, const item_loot_table *d)
      : m_name{n}, m_char{c}, m_drops{d} {}

  [[nodiscard]] constexpr auto character() const noexcept { return m_char; }
  [[nodiscard]] constexpr auto carry() const noexcept { return m_carry; }
  [[nodiscard]] constexpr auto drops() const noexcept { return m_drops; }
  [[nodiscard]] constexpr auto name() const noexcept { return m_name; }
  [[nodiscard]] constexpr auto attack() const noexcept { return m_attack; }
  [[nodiscard]] constexpr auto defense() const noexcept { return m_defense; }
  [[nodiscard]] constexpr auto life_gain() const noexcept {
    return m_life_gain;
  }
  [[nodiscard]] constexpr auto light_provided() const noexcept {
    return m_light_provided;
  }

  [[nodiscard]] constexpr bool operator==(const item_type &o) const noexcept {
    return m_char == o.m_char;
  }

  [[nodiscard]] constexpr item_type nullify() const noexcept {
    return item_type{};
  }

  [[nodiscard]] constexpr item_type attack(int a) const noexcept {
    auto r = *this;
    r.m_attack = a;
    r.m_carry = carry_one;
    return r;
  }
  [[nodiscard]] constexpr item_type defense(int a) const noexcept {
    auto r = *this;
    r.m_defense = a;
    r.m_carry = carry_one;
    return r;
  }
  [[nodiscard]] constexpr item_type defense_pair(int a) const noexcept {
    auto r = *this;
    r.m_defense = a;
    r.m_carry = carry_two;
    return r;
  }
  [[nodiscard]] constexpr item_type light_provided(int a) const noexcept {
    auto r = *this;
    r.m_light_provided = a;
    return r;
  }
  [[nodiscard]] constexpr item_type restores(int a) const noexcept {
    auto r = *this;
    r.m_life_gain = a;
    return r;
  }
  [[nodiscard]] constexpr item_type inventory_at(unsigned x,
                                                 unsigned y) const noexcept {
    auto r = *this;
    r.m_inv_coords = {x, y};
    return r;
  }
};

// Missing in previous code. They are "nullified" until all code is migrated.
// Then missing parts can be added or these can be entirely removed
constexpr const auto cloth =
    item_type{"linen tunic", 'a'}.defense(1).inventory_at(1, 3).nullify();
constexpr const auto leather =
    item_type{"leather vest", 'b'}.defense(2).inventory_at(1, 4).nullify();
constexpr const auto sword = item_type{"short sword", 'c'}.attack(12).nullify();

constexpr const auto bag = item_type{"bag", 'd', carry_one};

constexpr const auto knife = item_type{"rust knife", 'e'}.attack(2);
constexpr const auto sickle = item_type{"sickle", 'f'}.attack(3);
constexpr const auto adze = item_type{"adze", 'g'}.attack(5);
constexpr const auto axe = item_type{"axe", 'h'}.attack(8);
constexpr const auto doubleaxe = item_type{"battle axe", 'i'}.attack(10);
constexpr const auto spear = item_type{"spear", 'j'}.attack(15);

constexpr const auto driedfruit =
    item_type{"strange fruit", 'k'}.restores(3).inventory_at(0, 0);
constexpr const auto cheese =
    item_type{"cheese", 'l'}.restores(5).inventory_at(0, 1);
constexpr const auto rawmeat =
    item_type{"raw meat", 'm'}.restores(8).inventory_at(0, 2);

constexpr const auto torch =
    item_type{"torch", 'n'}.light_provided(10).inventory_at(0, 3);
constexpr const auto candle =
    item_type{"candle", 'o'}.light_provided(15).inventory_at(0, 4);
constexpr const auto oillamp =
    item_type{"lamp", 'p'}.light_provided(25).inventory_at(0, 5);

constexpr const auto greave =
    item_type{"boots", 'q'}.defense_pair(1).inventory_at(1, 0);
constexpr const auto armguard =
    item_type{"gloves", 'r'}.defense_pair(1).inventory_at(1, 1);
constexpr const auto pauldron =
    item_type{"helmet", 's'}.defense(1).inventory_at(1, 2);
constexpr const auto scale =
    item_type{"chest armor", 't'}.defense(3).inventory_at(1, 3);
constexpr const auto shield =
    item_type{"shield", 'u'}.defense(5).inventory_at(1, 4);

constexpr const auto jar_drops = item_loot_table{
    item_loot_table::roll_t{&driedfruit, &candle, &cheese},
    item_loot_table::roll_t{&driedfruit, &candle, &cheese},
    item_loot_table::roll_t{&driedfruit, &candle, &cheese},
    item_loot_table::roll_t{&driedfruit, &candle, &cheese},
    item_loot_table::roll_t{&driedfruit, &candle, &cheese},
    item_loot_table::roll_t{&driedfruit, &candle, &cheese},
    item_loot_table::roll_t{&driedfruit, &candle, &cheese},
    item_loot_table::roll_t{&driedfruit, &candle},
    item_loot_table::roll_t{&candle, &cheese},
    item_loot_table::roll_t{&driedfruit, &candle},
    item_loot_table::roll_t{&cheese},
    item_loot_table::roll_t{&driedfruit, &candle},
    item_loot_table::roll_t{&cheese},
    item_loot_table::roll_t{&driedfruit, &candle},
    item_loot_table::roll_t{&driedfruit, &cheese},
    item_loot_table::roll_t{&driedfruit, &candle, &cheese},
    item_loot_table::roll_t{&driedfruit, &cheese},
    item_loot_table::roll_t{&driedfruit, &candle, &cheese},
    item_loot_table::roll_t{&driedfruit, &cheese},
    item_loot_table::roll_t{&driedfruit, &candle, &cheese},
};
constexpr const auto chest_drops = item_loot_table{
    item_loot_table::roll_t{&cloth, &knife, &bag},
    item_loot_table::roll_t{&cloth, &knife, &bag},
    item_loot_table::roll_t{&leather, &knife, &bag},
    item_loot_table::roll_t{&cloth, &knife, &bag},
    item_loot_table::roll_t{&cloth, &sickle, &bag},
    item_loot_table::roll_t{&greave, &sickle, &bag},
    item_loot_table::roll_t{&greave, &sickle, &bag},
    item_loot_table::roll_t{&greave, &sickle, &bag},
    item_loot_table::roll_t{&armguard, &adze, &bag},
    item_loot_table::roll_t{&armguard, &adze, &bag},
    item_loot_table::roll_t{&armguard, &adze, &oillamp},
    item_loot_table::roll_t{&pauldron, &adze, &bag},
    item_loot_table::roll_t{&pauldron, &axe, &oillamp},
    item_loot_table::roll_t{&pauldron, &axe, &bag},
    item_loot_table::roll_t{&leather, &axe, &oillamp},
    item_loot_table::roll_t{&leather, &axe, &bag},
    item_loot_table::roll_t{&leather, &sword, &oillamp},
    item_loot_table::roll_t{&scale, &sword, &bag},
    item_loot_table::roll_t{&scale, &sword, &oillamp},
    item_loot_table::roll_t{&scale, &sword, &bag},
};

constexpr const auto jar = item_type{"jar", 'v', &jar_drops};
constexpr const auto coffer = item_type{"chest", 'w', &chest_drops};

static constexpr const auto item_type_count = 20;
constexpr const item_type *item_types[item_type_count] = {
    &adze,      &armguard,   &axe,    &bag,    &candle, &cheese,  &coffer,
    &doubleaxe, &driedfruit, &greave, &jar,    &knife,  &oillamp, &pauldron,
    &rawmeat,   &scale,      &shield, &sickle, &spear,  &torch,
};
} // namespace cno
