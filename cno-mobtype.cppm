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
  int m_life;
  int m_dice;
  hostilities m_hostility;
  int m_poison{};
  mob_drops m_drops;

public:
  constexpr mob_type(jute::view n, hostilities h, int d)
      : m_name{n}, m_hostility{h}, m_dice{d}, m_life{d} {}

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
constexpr const auto rat = mob_type{"rat", h_scaried, 6}.drops(&rawmeat);
constexpr const auto scorpion = mob_type{"scorpion", h_scaried, 4}.poison(8);
constexpr const auto centripede =
    mob_type{"centripede", h_scaried, 6}.poison(8);

constexpr const auto cerberus = mob_type{"cerberus", h_aggresive, 8};
constexpr const auto harpy = mob_type{"harpy", h_aggresive, 8};
constexpr const auto chimera = mob_type{"chimera", h_aggresive, 12}.poison(10);
constexpr const auto manticore = mob_type{"manticore", h_aggresive, 12};
constexpr const auto crocodile =
    mob_type{"crocodile", h_aggresive, 20}.drops(&rawmeat, &leather);
constexpr const auto drakon =
    mob_type{"drakon", h_aggresive, 20}.drops(&shield, &shield);
constexpr const auto boar =
    mob_type{"boar", h_aggresive, 8}.drops(&rawmeat, &rawmeat);
constexpr const auto griffin = mob_type{"griffin", h_aggresive, 12};
constexpr const auto sphinx = mob_type{"sphinx", h_aggresive, 12};
constexpr const auto bull =
    mob_type{"bull", h_aggresive, 12}.drops(&rawmeat, &rawmeat, &rawmeat);
} // namespace cno
