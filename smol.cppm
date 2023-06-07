export module smol;
import ecs;
import inv;
import map;
import mobs;
import pog;
import roll;
import qsu;

namespace smol {
export class game {
  qsu::renderer m_r{1};
  qsu::layout m_qsu{&m_r};

  ecs::ec m_ec;

  bool take_items() {
    auto pc = m_ec.coords.get(m_ec.player.get_id());
    m_ec.usables.remove_if([&](auto _, auto id) -> bool {
      if (pc != m_ec.coords.get(id))
        return false;

      ecs::remove_item(&m_ec, id);
      return false;
    });

    return true;
  }

  void move_player(int dx, int dy) {
    auto pid = m_ec.player.get_id();
    if (mobs::move_mob(&m_ec, pid, dx, dy))
      take_items();
  }

  bool check_exit() {
    auto pc = m_ec.coords.get(m_ec.player.get_id());
    auto ec = m_ec.coords.get(m_ec.exit.get_id());
    return ec == pc;
  }

  void move(int dx, int dy) {
    move_player(dx, dy);

    if (check_exit()) {
      auto pid = m_ec.player.get_id();
      m_ec.coords.update(pid, {1, 1});
    }

    m_qsu.fill_quack(&m_ec);
  }

  void show_all() {
    for (auto &[spr, _] : m_ec.sprites) {
      spr.alpha = 1.0;
    }
  }

public:
  void down() { move(0, 1); }
  void left() { move(-1, 0); }
  void right() { move(1, 0); }
  void up() { move(0, -1); }

  void use() {}
  void reset() {
    map::create_room(&m_ec, 5, 5);
    map::add_exit(&m_ec, 3, 3);

    auto enemy = ecs::add_hostile_enemy(&m_ec, 'B');
    ecs::set_mob_position(&m_ec, enemy, {2, 2});

    auto item = inv::items::add_cheese(&m_ec);
    m_ec.coords.update(item, {3, 2});

    ecs::add_player(&m_ec, 'A', {1, 1});

    show_all();
    m_qsu.fill_quack(&m_ec);
  }

  void process_event(const auto &e) {
    m_r.process_event(e);
    m_qsu.process_event(e);
  }
};
} // namespace smol