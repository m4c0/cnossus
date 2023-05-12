export module cno:mobtype;
import :entities;
import :itemtype;
import :random;
import :sprite;
import jute;

namespace cno {
constexpr const mob_type minotaur{
    .name = "you",
    .id = 'A',
    .life = 20,
    .dice = 10,
    .hostility = h_none,
};

constexpr const mob_type snake{
    .name = "snake",
    .id = 'B',
    .life = 8,
    .hostility = h_scaried,
    .poison = 6,
    .drops = {&rawmeat, &armguard},
};
constexpr const mob_type spider{
    .name = "spider",
    .id = 'C',
    .life = 4,
    .hostility = h_scaried,
    .poison = 4,
};
constexpr const mob_type rat{
    .name = "bat",
    .id = 'D',
    .life = 6,
    .hostility = h_scaried,
    .drops = {&rawmeat},
};
constexpr const mob_type scorpion{
    .name = "wasp",
    .id = 'E',
    .life = 4,
    .hostility = h_scaried,
    .poison = 8,
};
constexpr const mob_type centipede{
    .name = "centipede",
    .id = 'F',
    .life = 6,
    .hostility = h_scaried,
    .poison = 8,
};

constexpr const mob_type cerberus{
    .name = "zombie",
    .id = 'G',
    .life = 8,
    .hostility = h_aggresive,
};
constexpr const mob_type harpy{
    .name = "demon",
    .id = 'H',
    .life = 8,
    .hostility = h_aggresive,
};
constexpr const mob_type chimera{
    .name = "chimera",
    .id = 'I',
    .life = 12,
    .hostility = h_aggresive,
    .poison = 10,
};
constexpr const mob_type manticore{
    .name = "goblin",
    .id = 'J',
    .life = 12,
    .hostility = h_aggresive,
};
constexpr const mob_type croc{
    .name = "crocodile man",
    .id = 'K',
    .life = 20,
    .hostility = h_aggresive,
    .drops = {&rawmeat, &leather},
};
constexpr const mob_type drakon{
    .name = "dragon",
    .id = 'L',
    .life = 20,
    .hostility = h_aggresive,
    .drops = {&shield, &shield},
};
constexpr const mob_type boar{
    .name = "rat",
    .id = 'M',
    .life = 8,
    .hostility = h_aggresive,
    .drops = {&rawmeat, &rawmeat},
};
constexpr const mob_type griffin{
    .name = "ogre",
    .id = 'N',
    .life = 12,
    .hostility = h_aggresive,
};
constexpr const mob_type sphinx{
    .name = "evil human",
    .id = 'O',
    .life = 12,
    .hostility = h_aggresive,
};
constexpr const mob_type bull{
    .name = "mutant rat",
    .id = 'P',
    .life = 12,
    .hostility = h_aggresive,
    .drops = {&rawmeat, &rawmeat, &rawmeat},
};

static constexpr const auto max_mob_roll = 5;

using mob_rolls = rnd_roll_per_level<const mob_type *, max_mob_roll>;
static constexpr const mob_rolls mob_roll_per_level = {
    mob_rolls::roll_t{&snake, &boar, &cerberus},
    mob_rolls::roll_t{&centipede, &boar, &harpy},
    mob_rolls::roll_t{&snake, &cerberus, &rat},
    mob_rolls::roll_t{&centipede, &harpy, &rat},
    mob_rolls::roll_t{&snake, &cerberus, &boar},
    mob_rolls::roll_t{&centipede, &harpy, &boar},
    mob_rolls::roll_t{&snake, &cerberus, &rat},
    mob_rolls::roll_t{&spider, &rat, &harpy},
    mob_rolls::roll_t{&scorpion, &boar, &croc},
    mob_rolls::roll_t{&spider, &boar, &harpy},
    mob_rolls::roll_t{&scorpion, &croc, &rat, &rat},
    mob_rolls::roll_t{&spider, &rat, &cerberus, &harpy},
    mob_rolls::roll_t{&scorpion, &croc, &boar},
    mob_rolls::roll_t{&spider, &boar, &rat, &harpy},
    mob_rolls::roll_t{&manticore, &griffin, &croc},
    mob_rolls::roll_t{&griffin, &bull, &spider},
    mob_rolls::roll_t{&sphinx, &manticore, &bull},
    mob_rolls::roll_t{&sphinx, &chimera, &bull},
    mob_rolls::roll_t{&manticore, &chimera, &bull},
    mob_rolls::roll_t{&drakon, &griffin, &bull},
};

} // namespace cno
