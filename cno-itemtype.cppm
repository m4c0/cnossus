export module cno:itemtype;
import :entities;
import :random;
import jute;

namespace cno {
// Missing in previous code, but it was in original ZX code.
// Then missing parts can be added or these can be entirely removed
constexpr const item_type cloth{
    .name = "linen tunic",
    .id = 'a',
    .defense = 1,
};
constexpr const item_type leather{
    .name = "leather vest",
    .id = 'b',
    .defense = 2,
};
constexpr const item_type sword{
    .name = "short sword",
    .id = 'c',
    .attack = 12,
};

// Bag
constexpr const item_type bag{
    .name = "bag",
    .id = 'd',
};

// Attack items
constexpr const item_type knife{
    .name = "rust knife",
    .id = 'e',
    .attack = 2,
};
constexpr const item_type sickle{
    .name = "sickle",
    .id = 'f',
    .attack = 3,
};
constexpr const item_type adze{
    .name = "adze",
    .id = 'g',
    .attack = 5,
};
constexpr const item_type axe{
    .name = "axe",
    .id = 'h',
    .attack = 8,
};
constexpr const item_type doubleaxe{
    .name = "battle axe",
    .id = 'i',
    .attack = 10,
};
constexpr const item_type spear{
    .name = "spear",
    .id = 'j',
    .attack = 15,
};

// Food
constexpr const item_type driedfruit{
    .name = "strange fruit",
    .id = 'k',
    .life_gain = 3,
};
constexpr const item_type cheese{
    .name = "cheese",
    .id = 'l',
    .life_gain = 5,
};
constexpr const item_type rawmeat{
    .name = "raw meat",
    .id = 'm',
    .life_gain = 8,
};

// Lights
constexpr const item_type torch{
    .name = "torch",
    .id = 'n',
    .light_provided = 10,
};
constexpr const item_type candle{
    .name = "candle",
    .id = 'o',
    .light_provided = 15,
};
constexpr const item_type oillamp{
    .name = "lamp",
    .id = 'p',
    .light_provided = 25,
};

// Defense
constexpr const item_type greave{
    .name = "boots",
    .id = 'q',
    .defense = 1,
};
constexpr const item_type armguard{
    .name = "gloves",
    .id = 'r',
    .defense = 1,
};
constexpr const item_type pauldron{
    .name = "helmet",
    .id = 's',
    .defense = 1,
};
constexpr const item_type scale{
    .name = "chest armor",
    .id = 't',
    .defense = 3,
};
constexpr const item_type shield{
    .name = "shield",
    .id = 'u',
    .defense = 5,
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
    .id = 'v',
    .drops = &jar_drops,
};
constexpr const item_type coffer{
    .name = "chest",
    .id = 'w',
    .drops = &chest_drops,
};

static constexpr const auto item_type_count = 20;
constexpr const item_type *item_types[item_type_count] = {
    &adze,      &armguard,   &axe,    &bag,    &candle, &cheese,  &coffer,
    &doubleaxe, &driedfruit, &greave, &jar,    &knife,  &oillamp, &pauldron,
    &rawmeat,   &scale,      &shield, &sickle, &spear,  &torch,
};
} // namespace cno
