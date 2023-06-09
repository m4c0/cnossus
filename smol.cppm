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

  void move(int dx, int dy) {
    mobs::move_player(&m_ec, dx, dy);

    if (mobs::is_player_at_exit(&m_ec)) {
      auto pid = m_ec.player.get_id();
      m_ec.coords.update(pid, {1, 1});
      ecs::remove_level(&m_ec);
      create_level();
      show_all();
    }

    m_qsu.fill_quack(&m_ec);
  }

  void create_level() {
    map::create_room(&m_ec, 5, 5);
    map::add_exit(&m_ec, 3, 3);

    auto enemy = ecs::add_hostile_enemy(&m_ec, 'B');
    ecs::set_mob_position(&m_ec, enemy, {2, 2});

    auto item = inv::items::add_cheese(&m_ec);
    m_ec.coords.update(item, {3, 2});
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
    create_level();

    ecs::add_player(&m_ec);

    show_all();
    m_qsu.fill_quack(&m_ec);
  }

  void process_event(const auto &e) {
    m_r.process_event(e);
    m_qsu.process_event(e);
  }
};
} // namespace smol
