export module cno:enemy;
import :objects;
import :mob;
import :mobtype;
import :random;

namespace cno {
class enemy : public mob {
  int m_bonus;

  map_coord hunt_hero(map_coord p) { return move_from_hero(p, 1); }
  map_coord run_from_hero(map_coord p) { return move_from_hero(p, -1); }
  map_coord wander() {
    switch (cno::random(4)) {
    case 0:
      return move(0, -1);
    case 1:
      return move(0, 1);
    case 2:
      return move(-1, 0);
    default:
      return move(1, 0);
    }
  }

  map_coord move_from_hero(map_coord p, int s) {
    const auto &[mx, my] = coord();
    const auto &[px, py] = p;

    if (mx == px) {
      return move(0, my > py ? -s : s);
    } else if (my == py) {
      return move(mx > px ? -s : s, 0);
    } else {
      return wander();
    }
  }

  map_coord move(int dx, int dy) {
    const auto &[x, y] = coord();
    return {x + dx, y + dy};
  }

protected:
  [[nodiscard]] int attack_bonus() const noexcept override { return m_bonus; }
  [[nodiscard]] int defense_bonus() const noexcept override { return m_bonus; }
  [[nodiscard]] int damage_bonus() const noexcept override { return 0; }

  [[nodiscard]] map_coord next_move_with_light(map_coord player_pos,
                                               unsigned l) noexcept override {
    switch (type()->hostility()) {
    case h_none:
    case h_scaried:
      if (l < 1)
        return wander();
      else
        return run_from_hero(player_pos);
    case h_aggresive:
      if (l < 1)
        return hunt_hero(player_pos);
      else
        return wander();
    }
  }

public:
  enemy(const mob_type *t, map_coord c, unsigned level)
      : mob{t, c}, m_bonus{static_cast<int>(level) / 2} {}
};
} // namespace cno
