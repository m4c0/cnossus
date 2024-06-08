export module cno:game;
import :camera;
import :enemy;
import :light;
import :player;
import ecs;
import inv;
import mobs;
import qsu;

namespace cno {
class game {
  // player m_player{};
  // light m_light{};
  unsigned m_level{};

  ecs::ec m_ec;

  /*
void move_enemies() {
  auto pc = m_player.coord();

  m_mobs.for_each([this, pc](auto &m) {
    auto tgt = enemy{&m}.next_move_with_light(pc, m_light);
    if (tgt != m.coord)
      try_move(&m, tgt);
  });
}

  void consume(qsu::type<item_type> t) {
    if (!m_player.consume(t))
      return;

    m_light.consume(t);
    tick();
  }
  */

  [[nodiscard]] bool game_is_over() const {
    return !m_ec.player.get_id() || m_level == max_level + 1;
  }

  void move_hero(int dx, int dy) {
    if (game_is_over())
      return;

    mobs::move_player(&m_ec, dx, dy);
    if (mobs::is_player_at_exit(&m_ec)) {
      mobs::set_player_pos(&m_ec, m_level % 2 ? ecs::map_width - 2 : 1, 1);

      next_level();
    }

    tick();
  }

  void next_level() {
    auto lvl = m_level + 1;
    if (lvl == max_level + 1) {
      // TODO: game over
    }
    m_level = lvl;
    create_level();
  }

  void use_item() {
    /*
    auto pid = m_ec.player.get_id();
    auto pc = m_ec.coords.get(pid);

    auto id = m_ec.usables.get(pc);
    if (!id)
      return;

    if (m_ec.exit.has(id)) {
      next_level();
      return;
    }

    m_ec.usables.remove(id);
    m_ec.in_use.add(id, {});

    tick();
    */
  }

  void repaint() {
    // unsigned dist = m_light.visible_distance();
    update_rogueview(&m_ec);
    qsu::fill_quack(&m_ec);
  }

  void create_level() {
    ecs::remove_level(&m_ec);
    map::create_maze(&m_ec, m_level, ecs::map_width, ecs::map_height);
    ecs::add_level_items(&m_ec, m_level);
    ecs::add_level_mobs(&m_ec, m_level);
    // m_player.level_reset(m_level);
    repaint();
  }

  void tick() {
    // mobs::tick(&m_mobs);
    // move_enemies();
    // m_light.tick();
    repaint();
  }

public:
  void process_event(const auto &e) { center_camera(&m_ec); }

  void reset() {
    m_level = 1;

    ecs::add_player(&m_ec);
    create_level();
  }

  void use() {
    if (game_is_over())
      return;

    use_item();
  }

  void down() { move_hero(0, 1); }
  void left() { move_hero(-1, 0); }
  void right() { move_hero(1, 0); }
  void up() { move_hero(0, -1); }
};
} // namespace cno
