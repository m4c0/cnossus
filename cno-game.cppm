export module cno:game;
import :camera;
import :enemy;
import :light;
import :mobs;
import :player;
import ecs;
import map;
import qsu;
import roll;

namespace cno {
class game {
  qsu::renderer m_r{3};
  qsu::layout m_qsu{&m_r};
  item_list m_items{&m_r};
  mob_list m_mobs{&m_r};
  player m_player{};
  light m_light{};
  unsigned m_level{};

  ecs::ec m_ec;

  void create_enemies() {
    m_mobs.reset_grid();
    m_player.copy_mob_to(m_mobs.at(0));
    m_player.add_entity(&m_ec);

    for (auto i = 0; i < map::height - 1; i++) {
      qsu::type<mob_type> t = qsu::type{mob_roll_per_level.roll(m_level)};
      if (!t)
        continue;

      auto id = t->hostility == h_aggresive
                    ? add_hostile_enemy(&m_ec, t->id)
                    : add_non_hostile_enemy(&m_ec, t->id);
      auto [x, y] = m_ec.blockers.get(id);

      auto &mm = m_mobs.at(i + 1) = {t, {x, y}};
      enemy{&mm}.reset_level(m_level);
    }
  }

  void try_move(mob *m, map_coord tgt) {
    /*
    if (m_ec.walls.has({tgt.x, tgt.y}))
      return;

    auto [x, y] = m->coord;
    auto e = m_ec.blockers.get({x, y});

    auto attacked = m_mobs.find_at(tgt, [&](auto &mm) {
      auto drop = mobs::attack(*m, mm);
      if (drop.type)
        m_items.add(drop);
    });

    if (attacked)
      return;

    m->coord = tgt;

    m_ec.blockers.update(e, {tgt.x, tgt.y});
    m_ec.coords.update(e, {tgt.x, tgt.y});
    */
  }

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

  [[nodiscard]] bool game_is_over() const {
    return m_player.is_dead() || m_level == max_level + 1;
  }

  void move_hero(int dx, int dy) {
    if (game_is_over())
      return;

    const auto &[x, y] = m_player.coord();
    try_move(m_player.mob(), map_coord{x + dx, y + dy});
    tick();
  }

  void next_level() {
    auto lvl = m_level + 1;
    if (lvl == max_level + 1) {
      // TODO: game over
    }
    set_level(lvl);
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
    auto pc = m_player.coord();
    unsigned dist = m_light.visible_distance();
    update_rogueview(&m_ec);
    m_qsu.fill_quack(&m_ec);
  }

  void set_level(unsigned l) {
    m_level = l;
    map::create_maze(&m_ec, l);
    m_player.level_reset(l);
    roll::add_level_items(&m_ec, l);
    create_enemies();
    repaint();
  }

  void tick() {
    m_player.copy_mob_to(m_mobs.at(0));
    mobs::tick(&m_mobs);
    move_enemies();
    m_light.tick();
    repaint();
  }

public:
  void process_event(const auto &e) {
    m_r.process_event(e);
    m_qsu.process_event(e);

    // TODO: easy in the movement:
    auto pc = m_player.coord();
    m_qsu.center_view(pc);
  }

  void reset() {
    m_player = {};
    set_level(1);
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
