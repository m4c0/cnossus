export module cno:game;
import :enemy;
import :itemlist;
import :light;
import :mobs;
import :player;
import ecs;
import map;
import qsu;

namespace cno {
class game {
  qsu::renderer m_r{3};
  qsu::layout2 m_qsu{&m_r};
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

      auto id = add_enemy(&m_ec);
      auto [x, y] = m_ec.blockers.get(id);

      auto &mm = m_mobs.at(i + 1) = {t, {x, y}};
      enemy{&mm}.reset_level(m_level);
    }
  }

  [[nodiscard]] bool open_item_at(map_coord c) {
    return m_items.find_at(c, [this, c](auto &it) {
      auto drops = it.type->drops;
      auto nit = (drops == nullptr) ? it.type : qsu::type{drops->roll(m_level)};
      if (!nit) {
        it = {};
        return;
      }

      if (nit->id != it.type->id) {
        it.type = nit;
        return;
      }

      if (m_player.get_item(nit)) {
        it = {};
        tick();
        return;
      }
    });
  }

  void try_move(mob *m, map_coord tgt) {
    if (m_ec.blockers.has({tgt.x, tgt.y}))
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

    m_ec.blockers.remove(e);
    m_ec.blockers.put(e, {tgt.x, tgt.y});

    m_ec.coords.set(e, {tgt.x, tgt.y});
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

  void create_map() {
    map::maze m{};
    m.build_level(m_level);
    m.build_entities(&m_ec);
  }

  void create_items() {
    m_items.reset_grid();

    for (auto i = 1; i < map::height - 2; i++) {
      auto type = qsu::type{item_roll_per_level.roll(m_level)};
      if (!type) {
        continue;
      }

      auto id = add_item(&m_ec);
      auto [x, y] = m_ec.coords.get(id);
      m_items.add({type, {x, y}});
    }
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
    auto pc = m_player.coord();
    if (open_item_at(pc)) {
      return;
    }

    for (auto [_, id] : m_ec.usables) {
      auto c = m_ec.coords.get(id);
      if (c.x != pc.x || c.y != pc.y)
        continue;

      // This should be a different action. If we try to fetch an item from
      // ground and we fail and the item is over the stair, we move to the next
      // level.
      // TODO: fix this.
      if (m_ec.exit.has(id)) {
        next_level();
        return;
      }
    }

    tick();
  }

  void repaint() {
    auto pc = m_player.coord();
    unsigned dist = m_light.visible_distance();
    // TODO: another system for "rogue view"
    // m_map.update_rogueview(pc, dist);
    m_mobs.update_rogueview(pc, dist);
    m_items.update_rogueview(pc, dist);

    mobs::hide_mobs_in_fog(&m_mobs);

    m_qsu.fill_quack(&m_ec);
    m_mobs.fill_quack();
    m_items.fill_quack();
  }

  void set_level(unsigned l) {
    m_level = l;
    create_map();
    m_player.level_reset(l);
    create_items();
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
    m_items.process_event(e);
    m_mobs.process_event(e);

    // TODO: easy in the movement:
    auto pc = m_player.coord();
    m_qsu.center_view(pc);
    m_items.center_view(pc);
    m_mobs.center_view(pc);
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
