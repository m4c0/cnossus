export module smol;
import ecs;
import map;
import roll;
import qsu;

namespace smol {
export class game {
  qsu::renderer m_r{1};
  qsu::layout m_qsu{&m_r};

  ecs::ec m_ec;

  void show_all() {
    for (auto &[spr, _] : m_ec.sprites) {
      spr.alpha = 1.0;
    }
  }

public:
  void down() {}
  void left() {}
  void right() {}
  void up() {}

  void use() {}
  void reset() {
    map::create_room(&m_ec, 5, 5);
    map::add_exit(&m_ec, 3, 3);
    roll::add_level_items(&m_ec, 1);

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
