export module smol;
import dbg;
import ecs;
import inv;
import mapecs;
import mobs;
import pog;
import spr;
import qsu;

namespace smol {
export class game {
  ecs::ec m_ec;
  unsigned m_level;

  void move(int dx, int dy) {
    mobs::move_player(&m_ec, dx, dy);

    if (mobs::is_player_at_exit(&m_ec)) {
      m_level++;
      mobs::set_player_pos(&m_ec, 1, 1);
      create_level();
    }

    dbg::show_all_sprites(&m_ec, 1.0);
    qsu::fill_quack(&m_ec);
  }

  void create_level() {
    ecs::remove_level(&m_ec);

    map::create_maze(&m_ec, 1, 7, 7);

    auto mob = ecs::add_level_mob(&m_ec, m_level);
    if (mob)
      ecs::set_mob_position(&m_ec, mob, {3, 3});

    dbg::show_all_sprites(&m_ec, 1.0);
  }

public:
  void down() { move(0, 1); }
  void left() { move(-1, 0); }
  void right() { move(1, 0); }
  void up() { move(0, -1); }

  void use() {
    mobs::move_non_hostile_enemies(&m_ec);
    qsu::fill_quack(&m_ec);
  }
  void reset() {
    m_level = 1;
    ecs::add_player(&m_ec);

    create_level();

    qsu::fill_quack(&m_ec);
  }

  void process_event(const auto &e) {}
};
} // namespace smol
