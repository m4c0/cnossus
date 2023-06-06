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

  void create_map() {
    for (auto y = 0U; y < 5; y++) {
      for (auto x = 0U; x < 5; x++) {
        ecs::add_walkable_block(&m_ec, '.', {x, y});
      }
    }

    for (auto x = 0U; x < 5; x++) {
      ecs::add_rigid_block(&m_ec, '&', {x, 0});
      ecs::add_rigid_block(&m_ec, '&', {x, 4});
    }
    for (auto y = 0U; y < 5; y++) {
      ecs::add_rigid_block(&m_ec, '&', {0, y});
      ecs::add_rigid_block(&m_ec, '&', {4, y});
    }
    ecs::add_exit(&m_ec, '<', {3, 3});
  }
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
    create_map();
    roll::add_level_items(&m_ec, 1);

    show_all();
    m_qsu.fill_quack(&m_ec);
  }

  void process_event(const auto &e) {
    m_r.process_event(e);
    m_qsu.process_event(e);
  }
};
} // namespace smol
