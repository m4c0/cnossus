export module smol;
import dbg;
import ecs;
import inv;
import map;
import mobs;
import pog;
import roll;
import spr;
import qsu;

namespace smol {
export class game {
  qsu::renderer m_r{1};
  qsu::layout m_qsu{&m_r};

  ecs::ec m_ec;

  void move(int dx, int dy) {
    mobs::move_player(&m_ec, dx, dy);

    if (mobs::is_player_at_exit(&m_ec)) {
      mobs::set_player_pos(&m_ec, 1, 1);
      create_level();
    }

    m_qsu.fill_quack(&m_ec);
  }

  void create_level() {
    ecs::remove_level(&m_ec);

    map::create_maze(&m_ec, 1, 7, 7);

    ecs::set_mob_position(&m_ec, ecs::add_snake(&m_ec), {2, 3});

    auto item = ecs::add_cheese(&m_ec);
    m_ec.coords.update(item, {3, 2});

    ecs::set_mob_position(&m_ec, ecs::add_coffer(&m_ec), {3, 4});

    dbg::show_all_sprites(&m_ec, 1.0);
  }

public:
  void down() { move(0, 1); }
  void left() { move(-1, 0); }
  void right() { move(1, 0); }
  void up() { move(0, -1); }

  void use() {}
  void reset() {
    ecs::add_player(&m_ec);

    create_level();

    m_qsu.fill_quack(&m_ec);
  }

  void process_event(const auto &e) {
    m_r.process_event(e);
    m_qsu.process_event(e);
  }
};
} // namespace smol
