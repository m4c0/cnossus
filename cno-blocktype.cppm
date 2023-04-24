export module cno:blocktype;
import jute;

namespace cno {
class block_type {
  jute::view m_name{};
  bool m_can_walk{};
  bool m_transparent{};
  char m_character{};

public:
  constexpr block_type(jute::view n, char c) : m_name{n}, m_character{c} {}

  constexpr block_type walkable() const noexcept {
    auto v = *this;
    v.m_can_walk = true;
    return v;
  }
  constexpr block_type transparent() const noexcept {
    auto v = *this;
    v.m_transparent = true;
    return v;
  }
};

constexpr const block_type blank =
    block_type{"void", ' '}.transparent(); // void

// Varies with floor
constexpr const block_type dot = block_type{"floor", '.'}.walkable(); // floor
constexpr const block_type hash = block_type{"wall", '&'};            // wall

// Fixed textures
constexpr const block_type comma =
    block_type{"mosaic", ','}.walkable();                   // mosaic
constexpr const block_type tilda = block_type{"pool", '%'}; // pool

// Requires combining with "floor"
constexpr const block_type vbar =
    block_type{"knowledge", '#'}.transparent(); // column
constexpr const block_type star =
    block_type{"fountain", '$'}.transparent(); // basin
constexpr const block_type gt =
    block_type{"stairs", '<'}.transparent().walkable(); // stairs
constexpr const block_type andsign =
    block_type{"statue", '('}.transparent(); // statue
} // namespace cno
