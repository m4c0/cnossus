export module smol;
import ecs;
import map;
import pog;
import roll;
import qsu;

namespace smol {
export class game {
  qsu::renderer m_r{1};
  qsu::layout m_qsu{&m_r};

  ecs::ec m_ec;

  bool move_mob(auto id, int dx, int dy) {
    auto [x, y] = m_ec.coords.get(id);
    auto tx = x + dx;
    auto ty = y + dy;

    if (m_ec.walls.has({tx, ty}))
      return false;

    if (m_ec.blockers.has({tx, ty}))
      return false;

    ecs::set_mob_position(&m_ec, id, {tx, ty});
    return true;
  }

  void move_hero(int dx, int dy) {
    auto pid = m_ec.player.get_id();

    if (!move_mob(pid, dx, dy))
      return;

    auto c = m_ec.coords.get(pid);
    m_ec.usables.remove_if([&](auto _, auto id) -> bool {
      if (c != m_ec.coords.get(id))
        return false;

      return false;
    });

    m_qsu.fill_quack(&m_ec);
  }

  void show_all() {
    for (auto &[spr, _] : m_ec.sprites) {
      spr.alpha = 1.0;
    }
  }

public:
  void down() { move_hero(0, 1); }
  void left() { move_hero(-1, 0); }
  void right() { move_hero(1, 0); }
  void up() { move_hero(0, -1); }

  void use() {}
  void reset() {
    map::create_room(&m_ec, 5, 5);
    map::add_exit(&m_ec, 3, 3);
    roll::add_level_items(&m_ec, 1);

    ecs::add_player(&m_ec, 'A', {1, 1});

    auto enemy = ecs::add_hostile_enemy(&m_ec, 'B');
    ecs::set_mob_position(&m_ec, enemy, {2, 2});

    show_all();
    m_qsu.fill_quack(&m_ec);
  }

  void process_event(const auto &e) {
    m_r.process_event(e);
    m_qsu.process_event(e);
  }
};
} // namespace smol
