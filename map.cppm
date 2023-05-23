export module map;
export import :block;
export import :maze;
import ecs;

namespace map {
export void create_maze(ecs::ec *ec, unsigned lvl) {
  maze m{};
  m.build_level(lvl);
  m.build_entities(ec);
}
} // namespace map
