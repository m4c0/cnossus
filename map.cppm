export module map;
export import :block;
export import :maze;
import qsu;

namespace map {
using qsu = qsu::layout<qsu::sprite<block>, width, height>;

export void build(qsu *tgt, unsigned lvl) {
  maze m{};
  m.build_level(lvl);

  const auto &mm = m.data();
  for (auto y = 0U; y < height; y++) {
    for (auto x = 0U; x < width; x++) {
      tgt->at(y * width + x) = {
          .type = mm[y][x],
          .coord = {x, y},
      };
    }
  }
}
} // namespace map
