export module cno:itemtype;
import :globals;
import :random;
import jute;

namespace cno {
static constexpr const auto max_item_drops = 5;

struct inventory_pos {
  unsigned sec;
  unsigned row;
};

class item_type;
struct item_loot_table {
  const item_type *table[max_level][max_item_drops]{};
};

class item_type {
  jute::view m_name;
  int m_attack{};
  int m_defense{};
  int m_max_carry{-1}; // TODO: consider a aux enum for this logic
  int m_life_gain{};
  int m_light_provided{};
  inventory_pos m_inv_coords{~0U, ~0U};
  const item_loot_table *m_drops{};

  explicit constexpr item_type() : m_name{""} {}

public:
  explicit constexpr item_type(jute::view n) : m_name{n} {}
  constexpr item_type(jute::view n, int m) : m_name{n}, m_max_carry{m} {}
  constexpr item_type(jute::view n, const item_loot_table *d)
      : m_name{n}, m_drops{d} {}

  [[nodiscard]] constexpr auto max_carry() const noexcept {
    return m_max_carry;
  }
  [[nodiscard]] constexpr auto name() const noexcept { return m_name; }
  [[nodiscard]] constexpr auto attack() const noexcept { return m_attack; }
  [[nodiscard]] constexpr auto defense() const noexcept { return m_defense; }
  [[nodiscard]] constexpr auto life_gain() const noexcept {
    return m_life_gain;
  }
  [[nodiscard]] constexpr auto light_provided() const noexcept {
    return m_light_provided;
  }

  [[nodiscard]] constexpr auto consumable() const noexcept {
    return m_life_gain > 0 || m_light_provided > 0;
  }

  [[nodiscard]] const item_type *drop_for_level(unsigned l) const noexcept {
    if (m_drops == nullptr) {
      return this;
    }

    auto rd = cno::random(max_item_drops);
    auto *drop = m_drops->table[l - 1][rd];
    if (drop == nullptr || drop->name() == "") {
      using namespace jute::literals;
      g::update_status("The "_s + m_name + " crumbled to dust");
      return nullptr;
    }

    g::update_status("Something fells on the ground");
    return drop;
  }

  [[nodiscard]] constexpr item_type nullify() const noexcept {
    return item_type{};
  }

  [[nodiscard]] constexpr item_type attack(int a) const noexcept {
    auto r = *this;
    r.m_attack = a;
    r.m_max_carry = 1;
    return r;
  }
  [[nodiscard]] constexpr item_type defense(int a) const noexcept {
    auto r = *this;
    r.m_defense = a;
    r.m_max_carry = 1;
    return r;
  }
  [[nodiscard]] constexpr item_type defense_pair(int a) const noexcept {
    auto r = *this;
    r.m_defense = a;
    r.m_max_carry = 2;
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
    item_type{"linen tunic"}.defense(1).inventory_at(1, 3).nullify();
constexpr const auto leather =
    item_type{"leather vest"}.defense(2).inventory_at(1, 4).nullify();
constexpr const auto sword = item_type{"short sword"}.attack(12).nullify();

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

constexpr const auto jar_drops = item_loot_table{{
    {&driedfruit, &candle, &cheese},
    {&driedfruit, &candle, &cheese},
    {&driedfruit, &candle, &cheese},
    {&driedfruit, &candle, &cheese},
    {&driedfruit, &candle, &cheese},
    {&driedfruit, &candle, &cheese},
    {&driedfruit, &candle, &cheese},
    {&driedfruit, &candle},
    {&candle, &cheese},
    {&driedfruit, &candle},
    {&cheese},
    {&driedfruit, &candle},
    {&cheese},
    {&driedfruit, &candle},
    {&driedfruit, &cheese},
    {&driedfruit, &candle, &cheese},
    {&driedfruit, &cheese},
    {&driedfruit, &candle, &cheese},
    {&driedfruit, &cheese},
    {&driedfruit, &candle, &cheese},
}};
constexpr const auto chest_drops = item_loot_table{{
    {&cloth, &knife, &bag},       {&cloth, &knife, &bag},
    {&leather, &knife, &bag},     {&cloth, &knife, &bag},
    {&cloth, &sickle, &bag},      {&greave, &sickle, &bag},
    {&greave, &sickle, &bag},     {&greave, &sickle, &bag},
    {&armguard, &adze, &bag},     {&armguard, &adze, &bag},
    {&armguard, &adze, &oillamp}, {&pauldron, &adze, &bag},
    {&pauldron, &axe, &oillamp},  {&pauldron, &axe, &bag},
    {&leather, &axe, &oillamp},   {&leather, &axe, &bag},
    {&leather, &sword, &oillamp}, {&scale, &sword, &bag},
    {&scale, &sword, &oillamp},   {&scale, &sword, &bag},
}};

constexpr const auto jar = item_type{"jar", &jar_drops};
constexpr const auto coffer = item_type{"chest", &chest_drops};

static constexpr const auto item_type_count = 20;
constexpr const item_type *item_types[item_type_count] = {
    &adze,      &armguard,   &axe,    &bag,    &candle, &cheese,  &coffer,
    &doubleaxe, &driedfruit, &greave, &jar,    &knife,  &oillamp, &pauldron,
    &rawmeat,   &scale,      &shield, &sickle, &spear,  &torch,
};
} // namespace cno
