export module cno:map;
import :blocktype;
import :globals;
import :random;
import casein;
import quack;

namespace cno {
struct block {
  stype<block_type> type;
  map_coord coord;
  sprite_visibility vis;
};

class map : sbatch<block, map_width * map_height> {
public:
  using sbatch::sbatch;

  using sbatch::fill_quack;
  using sbatch::process_event;

  [[nodiscard]] constexpr auto &at(unsigned x, unsigned y) noexcept {
    return sbatch::at(y * map_width + x);
  }
};
} // namespace cno
