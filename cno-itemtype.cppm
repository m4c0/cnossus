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

struct item_type {
  jute::view name{};
  char character{};
  int attack{};
  int defense{};
  carry_type carry{carry_many};
  int life_gain{};
  int light_provided{};
  inventory_pos inv_coords{~0U, ~0U};
  const item_loot_table *drops{};
};
[[nodiscard]] constexpr bool operator==(const item_type &a,
                                        const item_type &b) noexcept {
  return a.character == b.character;
}

// Missing in previous code.
// Then missing parts can be added or these can be entirely removed
constexpr const item_type cloth{
    .name = "linen tunic",
    .character = 'a',
    .defense = 1,
    .carry = carry_one,
    .inv_coords = {1, 5},
};
constexpr const item_type leather{
    .name = "leather vest",
    .character = 'b',
    .defense = 2,
    .carry = carry_one,
    .inv_coords = {1, 6},
};
constexpr const item_type sword{
    .name = "short sword",
    .character = 'c',
    .attack = 12,
    .carry = carry_one,
};

// Bag
constexpr const item_type bag{
    .name = "bag",
    .character = 'd',
    .carry = carry_one,
};

// Attack items
constexpr const item_type knife{
    .name = "rust knife",
    .character = 'e',
    .attack = 2,
};
constexpr const item_type sickle{
    .name = "sickle",
    .character = 'f',
    .attack = 3,
};
constexpr const item_type adze{
    .name = "adze",
    .character = 'g',
    .attack = 5,
};
constexpr const item_type axe{
    .name = "axe",
    .character = 'h',
    .attack = 8,
};
constexpr const item_type doubleaxe{
    .name = "battle axe",
    .character = 'i',
    .attack = 10,
};
constexpr const item_type spear{
    .name = "spear",
    .character = 'j',
    .attack = 15,
};

// Food
constexpr const item_type driedfruit{
    .name = "strange fruit",
    .character = 'k',
    .life_gain = 3,
    .inv_coords = {0, 0},
};
constexpr const item_type cheese{
    .name = "cheese",
    .character = 'l',
    .life_gain = 5,
    .inv_coords = {0, 1},
};
constexpr const item_type rawmeat{
    .name = "raw meat",
    .character = 'm',
    .life_gain = 8,
    .inv_coords = {0, 2},
};

// Lights
constexpr const item_type torch{
    .name = "torch",
    .character = 'n',
    .light_provided = 10,
    .inv_coords = {0, 3},
};
constexpr const item_type candle{
    .name = "candle",
    .character = 'o',
    .light_provided = 15,
    .inv_coords = {0, 3},
};
constexpr const item_type oillamp{
    .name = "lamp",
    .character = 'p',
    .light_provided = 25,
    .inv_coords = {0, 4},
};

// Defense
constexpr const item_type greave{
    .name = "boots",
    .character = 'q',
    .defense = 1,
    .carry = carry_two,
    .inv_coords = {1, 0},
};
constexpr const item_type armguard{
    .name = "gloves",
    .character = 'r',
    .defense = 1,
    .carry = carry_two,
    .inv_coords = {1, 1},
};
constexpr const item_type pauldron{
    .name = "helmet",
    .character = 's',
    .defense = 1,
    .carry = carry_one,
    .inv_coords = {1, 2},
};
constexpr const item_type scale{
    .name = "chest armor",
    .character = 't',
    .defense = 3,
    .carry = carry_one,
    .inv_coords = {1, 3},
};
constexpr const item_type shield{
    .name = "shield",
    .character = 'u',
    .defense = 5,
    .inv_coords = {1, 4},
};

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

constexpr const item_type jar{
    .name = "jar",
    .character = 'v',
    .drops = &jar_drops,
};
constexpr const item_type coffer{
    .name = "chest",
    .character = 'w',
    .drops = &chest_drops,
};

static constexpr const auto item_type_count = 20;
constexpr const item_type *item_types[item_type_count] = {
    &adze,      &armguard,   &axe,    &bag,    &candle, &cheese,  &coffer,
    &doubleaxe, &driedfruit, &greave, &jar,    &knife,  &oillamp, &pauldron,
    &rawmeat,   &scale,      &shield, &sickle, &spear,  &torch,
};
} // namespace cno
