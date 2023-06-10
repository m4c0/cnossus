export module ecs:containers;
import :ec;
import :entities;
import spr;

export namespace ecs {
auto add_jar(ecs::ec *ec) {
  auto e = add_container_item(ec, spr::jar);
  return e;
}
// chest
auto add_coffer(ecs::ec *ec) {
  auto e = add_container_item(ec, spr::coffer);
  return e;
}
} // namespace ecs
