export module cno:mobtype;
import :itemtype;
import :random;
import jute;

namespace cno {
static constexpr const auto max_mob_drops = 3;

enum hostilities { h_none, h_scaried, h_aggresive };

using mob_drops = rnd_roll<const item_type *, max_mob_drops>;

class mob_type {
  jute::view m_name;
  char m_char;
  unsigned m_life;
  unsigned m_dice;
  hostilities m_hostility;
  unsigned m_poison{};
  mob_drops m_drops;

public:
  constexpr mob_type(jute::view n, char c, hostilities h, unsigned d)
      : m_name{n}, m_char{c}, m_hostility{h}, m_dice{d}, m_life{d} {}

  [[nodiscard]] constexpr auto character() const noexcept { return m_char; }
  [[nodiscard]] constexpr auto dice() const noexcept { return m_dice; }
  [[nodiscard]] constexpr auto &drops() const noexcept { return m_drops; }
  [[nodiscard]] constexpr auto hostility() const noexcept {
    return m_hostility;
  }
  [[nodiscard]] constexpr auto life() const noexcept { return m_life; }
  [[nodiscard]] constexpr auto name() const noexcept { return m_name; }
  [[nodiscard]] constexpr auto poison() const noexcept { return m_poison; }

  [[nodiscard]] constexpr mob_type drops(auto... d) const noexcept {
    auto r = *this;
    r.m_drops = {d...};
    return r;
  }
  [[nodiscard]] constexpr mob_type life(unsigned l) const noexcept {
    auto r = *this;
    r.m_life = l;
    return r;
  }
  [[nodiscard]] constexpr mob_type poison(unsigned l) const noexcept {
    auto r = *this;
    r.m_poison = l;
    return r;
  }
};

[[nodiscard]] constexpr bool operator==(const mob_type &a,
                                        const mob_type &b) noexcept {
  return a.character() == b.character();
}
[[nodiscard]] constexpr auto is_player(const mob_type *a) noexcept {
  return a->hostility() == h_none;
}

constexpr const auto minotaur = mob_type{"you", 'A', h_none, 10}.life(20);

constexpr const auto snake =
    mob_type{"snake", 'B', h_scaried, 8}.poison(6).drops(&rawmeat, &armguard);
constexpr const auto spider = mob_type{"spider", 'C', h_scaried, 4}.poison(4);
constexpr const auto rat = mob_type{"bat", 'D', h_scaried, 6}.drops(&rawmeat);
constexpr const auto scorpion = mob_type{"wasp", 'E', h_scaried, 4}.poison(8);
constexpr const auto centipede =
    mob_type{"centipede", 'F', h_scaried, 6}.poison(8);

constexpr const auto cerberus = mob_type{"zombie", 'G', h_aggresive, 8};
constexpr const auto harpy = mob_type{"demon", 'H', h_aggresive, 8};
constexpr const auto chimera =
    mob_type{"chimera", 'I', h_aggresive, 12}.poison(10);
constexpr const auto manticore = mob_type{"goblin", 'J', h_aggresive, 12};
constexpr const auto croc =
    mob_type{"crocodile man", 'K', h_aggresive, 20}.drops(&rawmeat, &leather);
constexpr const auto drakon =
    mob_type{"dragon", 'L', h_aggresive, 20}.drops(&shield, &shield);
constexpr const auto boar =
    mob_type{"rat", 'M', h_aggresive, 8}.drops(&rawmeat, &rawmeat);
constexpr const auto griffin = mob_type{"ogre", 'N', h_aggresive, 12};
constexpr const auto sphinx = mob_type{"evil human", 'O', h_aggresive, 12};
constexpr const auto bull = mob_type{"mutant rat", 'P', h_aggresive, 12}.drops(
    &rawmeat, &rawmeat, &rawmeat);

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
