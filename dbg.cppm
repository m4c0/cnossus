export module dbg;
import ecs;

namespace dbg {
export void show_all_sprites(ecs::ec *ec, float alpha = 1.0) {
  for (auto &[spr, _] : ec->sprites) {
    spr.alpha = alpha;
  }
}
} // namespace dbg
