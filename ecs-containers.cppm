export module ecs:containers;
import :ec;
import :entities;
import spr;

export namespace ecs {
auto add_jar(ecs::ec *ec) { return add_container_item(ec, spr::jar); }
// chest
auto add_coffer(ecs::ec *ec) { return add_container_item(ec, spr::coffer); }
} // namespace ecs
