export module cno:camera;
import ecs;
import qsu;

namespace cno {
void update_rogueview(ecs::ec *ec) {
  auto sq_light = 2 * 2;

  auto pid = ec->player.get_id();
  auto [px, py] = ec->coords.get(pid);

  for (auto &[sid, spr] : ec->sprites) {
    auto [sx, sy] = ec->coords.get(sid);
    auto dx = sx - px;
    auto dy = sy - py;
    auto sq_dist = dx * dx + dy * dy;
    if (sq_dist <= sq_light) {
      spr.alpha = 1.0;
      spr.seen = true;
    } else if (spr.seen && !ec->enemies.has(sid)) {
      spr.alpha = 0.5;
    } else {
      spr.alpha = 0.0;
    }
  }
}

void center_camera(qsu::layout *qsu, ecs::ec *ec) {
  auto pid = ec->player.get_id();
  if (!pid)
    return;

  // TODO: easy in movement
  auto [x, y] = ec->coords.get(pid);
  qsu->center_view(x, y);
}
} // namespace cno
