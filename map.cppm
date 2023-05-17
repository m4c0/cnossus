export module map;
export import :block;
export import :maze;
import qsu;

namespace map {
using qsu = qsu::layout<qsu::sprite<block>, width, height>;

export void build(qsu *tgt, unsigned lvl) {
  maze m{};
  m.build_level(lvl);
  m.build_sprites(tgt);
}
} // namespace map
