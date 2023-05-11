export module cno:map;
import :blocktype;
import :globals;
import :random;
import casein;
import quack;

namespace cno {
enum block_vibility { bv_none, bv_fog, bv_visible };
struct block {
  sprite<block_type> type;
  map_coord coord;
  block_vibility vis;
};

class map : sbatch<block, map_width * map_height> {
  void update_rogueview(map_coord pc, unsigned radius) noexcept {
    for (auto x = 0U; x < map_width; x++) {
      auto dx = pc.x - x;
      for (auto y = 0U; y < map_height; y++) {
        auto dy = pc.y - y;
        auto &blk = at(x, y);
        blk.coord = {x, y};
        if (dx * dx + dy * dy <= radius * radius) {
          blk.vis = bv_visible;
        } else if (blk.vis == bv_visible) {
          blk.vis = bv_fog;
        }
      }
    }
  }

public:
  using sbatch::sbatch;

  using sbatch::process_event;

  // TODO: migrate to sbatch
  [[nodiscard]] constexpr block &at(unsigned x, unsigned y) noexcept {
    return sbatch::at(y * map_width + x);
  }
  [[nodiscard]] constexpr block at(unsigned x, unsigned y) const noexcept {
    return sbatch::at(y * map_width + x);
  }

  void fill_quack(map_coord pc, unsigned d) noexcept {
    update_rogueview(pc, d);

    sbatch::fill_quack(pc, d);
    fill_mult([](const block &blk) {
      switch (blk.vis) {
      case bv_visible:
        return quack::colour{1, 1, 1, 1};
      case bv_none:
        return quack::colour{1, 1, 1, 0};
      default:
        return quack::colour{1, 1, 1, 0.8f};
      }
    });
  }
};
} // namespace cno
