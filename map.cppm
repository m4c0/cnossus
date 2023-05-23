export module map;
export import :block;
export import :maze;
import ecs;

namespace map {
export void create_maze(ecs::ec *ec, unsigned lvl) {
  maze{ec}.build_level(lvl);
}
} // namespace map
