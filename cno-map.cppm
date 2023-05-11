export module cno:map;
import :blocktype;
import :globals;
import :random;
import casein;
import quack;

namespace cno {
using block = sprite<block_type>;

struct map : sbatch<block, map_width * map_height> {
public:
  using sbatch::sbatch;

  [[nodiscard]] constexpr auto &at(unsigned x, unsigned y) noexcept {
    return sbatch::at(y * map_width + x);
  }
};
} // namespace cno
