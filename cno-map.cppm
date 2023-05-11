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
  block_vibility vis;
};

class map {
  quack::grid_ilayout<map_width, map_height, block> m_blocks;

  void update_rogueview(map_coord pc, unsigned radius) noexcept {
    for (auto x = 0; x < map_width; x++) {
      auto dx = pc.x - x;
      for (auto y = 0; y < map_height; y++) {
        auto dy = pc.y - y;
        auto &vis = m_blocks.at(x, y).vis;
        if (dx * dx + dy * dy <= radius * radius) {
          vis = bv_visible;
        } else if (vis == bv_visible) {
          vis = bv_fog;
        }
      }
    }
  }

public:
  constexpr map(quack::renderer *r) : m_blocks{r} {}

  // TODO: migrate to sbatch
  [[nodiscard]] constexpr block &at(unsigned x, unsigned y) noexcept {
    return m_blocks.at(x, y);
  }
  [[nodiscard]] constexpr block at(unsigned x, unsigned y) const noexcept {
    return m_blocks.at(x, y);
  }

  void fill_quack(map_coord pc, unsigned d) noexcept {
    update_rogueview(pc, d);

    m_blocks.fill_uv(
        [](auto &i) { return i.type ? i.type->id.uv() : quack::uv{}; });
    m_blocks.fill_colour([](const block &blk) { return quack::colour{}; });
    m_blocks.fill_mult([](const block &blk) {
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

  void process_event(const casein::event &e) noexcept {
    m_blocks.process_event(e);
  }
};
} // namespace cno
