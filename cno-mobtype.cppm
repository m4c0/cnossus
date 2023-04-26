export module cno:mobtype;
import :itemtype;
import jute;

namespace cno {
static constexpr const auto max_mob_drops = 3;

enum hostilities { h_none, h_scaried, h_aggresive };
struct mob_drops {
  const item_type *table[max_mob_drops]{};
};

class mob_type {
  jute::view m_name;
  unsigned m_life;
  unsigned m_dice;
  hostilities m_hostility;
  unsigned m_poison{};
  mob_drops m_drops;

public:
  constexpr mob_type(jute::view n, hostilities h, unsigned d)
      : m_name{n}, m_hostility{h}, m_dice{d}, m_life{d} {}

  [[nodiscard]] constexpr auto life() const noexcept { return m_life; }

  [[nodiscard]] int dice_roll(int dice) const noexcept {
    auto roll = 0;
    for (auto i = 0; i < dice; i++) {
      roll += 1 + cno::random(m_dice);
    }
    return roll;
  }
  [[nodiscard]] const item_type *random_drop() const noexcept {
    return m_drops.table[cno::random(max_mob_drops)];
  }

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

constexpr const auto minotaur = mob_type{"you", h_none, 10}.life(20);

constexpr const auto snake =
    mob_type{"snake", h_scaried, 8}.poison(6).drops(&rawmeat, &armguard);
constexpr const auto spider = mob_type{"spider", h_scaried, 4}.poison(4);
constexpr const auto rat = mob_type{"bat", h_scaried, 6}.drops(&rawmeat);
constexpr const auto scorpion = mob_type{"wasp", h_scaried, 4}.poison(8);
constexpr const auto centripede =
    mob_type{"centripede", h_scaried, 6}.poison(8);

constexpr const auto cerberus = mob_type{"zombie", h_aggresive, 8};
constexpr const auto harpy = mob_type{"demon", h_aggresive, 8};
constexpr const auto chimera = mob_type{"chimera", h_aggresive, 12}.poison(10);
constexpr const auto manticore = mob_type{"goblin", h_aggresive, 12};
constexpr const auto crocodile =
    mob_type{"crocodile man", h_aggresive, 20}.drops(&rawmeat, &leather);
constexpr const auto drakon =
    mob_type{"dragon", h_aggresive, 20}.drops(&shield, &shield);
constexpr const auto boar =
    mob_type{"rat", h_aggresive, 8}.drops(&rawmeat, &rawmeat);
constexpr const auto griffin = mob_type{"ogre", h_aggresive, 12};
constexpr const auto sphinx = mob_type{"evil human", h_aggresive, 12};
constexpr const auto bull =
    mob_type{"mutant rat", h_aggresive, 12}.drops(&rawmeat, &rawmeat, &rawmeat);
} // namespace cno
