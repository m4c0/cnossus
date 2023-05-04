export module cno:mobtype;
import :itemtype;
import :random;
import jute;

namespace cno {
static constexpr const auto max_mob_drops = 3;

enum hostilities { h_none, h_scaried, h_aggresive };

using mob_drops = rnd_roll<const item_type *, max_mob_drops>;

struct mob_type {
  jute::view name;
  char character;
  unsigned life;
  unsigned dice = life;
  hostilities hostility;
  unsigned poison{};
  mob_drops drops{};
};

[[nodiscard]] constexpr bool operator==(const mob_type &a,
                                        const mob_type &b) noexcept {
  return a.character == b.character;
}
[[nodiscard]] constexpr auto is_player(const mob_type *a) noexcept {
  return a->hostility == h_none;
}

constexpr const mob_type minotaur{
    .name = "you",
    .character = 'A',
    .life = 20,
    .dice = 10,
    .hostility = h_none,
};

constexpr const mob_type snake{
    .name = "snake",
    .character = 'B',
    .life = 8,
    .hostility = h_scaried,
    .poison = 6,
    .drops = {&rawmeat, &armguard},
};
constexpr const mob_type spider{
    .name = "spider",
    .character = 'C',
    .life = 4,
    .hostility = h_scaried,
    .poison = 4,
};
constexpr const mob_type rat{
    .name = "bat",
    .character = 'D',
    .life = 6,
    .hostility = h_scaried,
    .drops = {&rawmeat},
};
constexpr const mob_type scorpion{
    .name = "wasp",
    .character = 'E',
    .life = 4,
    .hostility = h_scaried,
    .poison = 8,
};
constexpr const mob_type centipede{
    .name = "centipede",
    .character = 'F',
    .life = 6,
    .hostility = h_scaried,
    .poison = 8,
};

constexpr const mob_type cerberus{
    .name = "zombie",
    .character = 'G',
    .life = 8,
    .hostility = h_aggresive,
};
constexpr const mob_type harpy{
    .name = "demon",
    .character = 'H',
    .life = 8,
    .hostility = h_aggresive,
};
constexpr const mob_type chimera{
    .name = "chimera",
    .character = 'I',
    .life = 12,
    .hostility = h_aggresive,
    .poison = 10,
};
constexpr const mob_type manticore{
    .name = "goblin",
    .character = 'J',
    .life = 12,
    .hostility = h_aggresive,
};
constexpr const mob_type croc{
    .name = "crocodile man",
    .character = 'K',
    .life = 20,
    .hostility = h_aggresive,
    .drops = {&rawmeat, &leather},
};
constexpr const mob_type drakon{
    .name = "dragon",
    .character = 'L',
    .life = 20,
    .hostility = h_aggresive,
    .drops = {&shield, &shield},
};
constexpr const mob_type boar{
    .name = "rat",
    .character = 'M',
    .life = 8,
    .hostility = h_aggresive,
    .drops = {&rawmeat, &rawmeat},
};
constexpr const mob_type griffin{
    .name = "ogre",
    .character = 'N',
    .life = 12,
    .hostility = h_aggresive,
};
constexpr const mob_type sphinx{
    .name = "evil human",
    .character = 'O',
    .life = 12,
    .hostility = h_aggresive,
};
constexpr const mob_type bull{
    .name = "mutant rat",
    .character = 'P',
    .life = 12,
    .hostility = h_aggresive,
    .drops = {&rawmeat, &rawmeat, &rawmeat},
};

static constexpr const auto max_mobs_per_level = map_height * 2;
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
