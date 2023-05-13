export module cno:game;
import :atlas;
import :enemy;
import :inventory;
import :itemlist;
import :labyrinth;
import :light;
import :map;
import :moblist;
import :player;
import casein;
import jute;
import qsu;
import quack;

namespace cno {
class game {
  quack::renderer m_r{3};
  map m_map{&m_r};
  item_list m_items{&m_r};
  mob_list m_mobs{&m_r};
  inv::table m_inv{};
  player m_player{&m_mobs.at(0)};
  light m_light{};
  unsigned m_level{};

  void create_enemies() {
    map_coord c{};
    for (c.y = 1; c.y < map_height - 1; c.y++) {
      const mob_type *t = mob_roll_per_level.roll(m_level);
      if (t == nullptr) {
        m_mobs.at(c.y) = {};
        continue;
      }

      do {
        c.x = cno::random(map_width);
      } while (!map_at(&m_map, c).type->can_walk);

      auto &mm = m_mobs.at(c.y) = {qsu::type{t}, c};
      enemy{&mm}.reset_level(m_level);
    }
  }

  [[nodiscard]] bool open_item_at(map_coord c) {
    return m_items.find_at(c, [this, c](auto &it) {
      auto drops = it.type->drops;
      auto nit = (drops == nullptr) ? it.type : qsu::type{drops->roll(m_level)};
      if (!nit) {
        using namespace jute::literals;
        g::update_status("The "_s + it.type->name + " crumbled to dust");
        it = {};
        return;
      }

      if (nit->id != it.type->id) {
        g::update_status("Something fells on the ground");
        it.type = nit;
        return;
      }

      if (m_inv.get_item(nit)) {
        it = {};
        m_player.update_inventory(m_inv);
        tick();
        return;
      }
    });
  }

  void try_move(mob *m, map_coord tgt) {
    auto blk = map_at(&m_map, tgt);
    if (!blk.type->can_walk) {
      if (is_player(*m)) {
        using namespace jute::literals;
        g::update_status("A "_s + blk.type->name + " blocks your way");
      }
      return;
    }

    auto attacked = m_mobs.find_at(tgt, [&](auto &mm) {
      if (m->type->id != mm.type->id) {
        attack(*m, mm);
      }
    });

    if (!attacked)
      m->coord = tgt;
  }

  void process_actions_with_light() {
    auto pc = m_player.coord();

    m_mobs.for_each([this, pc](auto &m) {
      if (m.life == 0)
        return;

      if (m.actions > 0) {
        m.actions -= m.type->dice;
        return;
      }

      if (m.poison > 0) {
        --m.poison;
        --m.life;
      }

      m.actions.to_max();

      auto tgt = enemy{&m}.next_move_with_light(pc, m_light);
      if (tgt != m.coord)
        try_move(&m, tgt);
    });
  }

  void attack(const mob &src, mob &tgt) {
    const auto srcn = src.type->name;
    const auto tgtn = tgt.type->name;

    int atk_roll = roll_dice(src.type->dice, 2) + src.bonus.attack;
    int def_roll = roll_dice(tgt.type->dice, 2) + tgt.bonus.defense;
    int margin = atk_roll - def_roll;

    if (margin > 0) {
      margin += src.bonus.attack - tgt.bonus.defense;
      if (margin <= 0)
        return;

      tgt.damage_timer = 0.5;
      tgt.life -= (tgt.life <= margin) ? tgt.life : margin;
      if (tgt.life == 0) {
        auto drop = qsu::type{tgt.type->drops.roll()};
        if (drop)
          m_items.add({drop, tgt.coord});
        if (is_player(src)) {
          g::update_status("You killed a " + tgtn);
        } else if (is_player(tgt)) {
          g::update_status("A " + srcn + " killed you");
        }
        tgt = {};
      } else if (src.type->poison > 0) {
        tgt.poison += 1 + cno::random(src.type->poison);
        if (is_player(tgt)) {
          g::update_status("A " + srcn + " poisons you");
        }
      } else if (is_player(src)) {
        g::update_status("You hit a " + tgtn);
      } else if (is_player(tgt)) {
        g::update_status("A " + srcn + " hits you");
      }
    } else if (margin == 0) {
      if (src.type->poison > 0) {
        tgt.poison += 1 + cno::random(src.type->poison);
        if (is_player(src)) {
          g::update_status("A " + srcn + " poisons you");
        }
      } else if (is_player(src)) {
        g::update_status("You barely miss " + tgtn);
      } else if (is_player(tgt)) {
        g::update_status("A " + srcn + " barely misses you");
      }
    } else if (is_player(src)) {
      g::update_status("You miss a " + tgtn);
    } else if (is_player(tgt)) {
      g::update_status("A " + srcn + " misses you");
    }
  }

  void consume(qsu::type<item_type> t) {
    if (!m_inv.consume(t))
      return;

    m_player.consume(t);
    m_light.consume(t);
    tick();
  }

  [[nodiscard]] bool game_is_over() const {
    return m_player.is_dead() || m_level == max_level + 1;
  }

  void create_items() {
    m_items.reset_grid();

    map_coord c{};
    for (c.y = 1; c.y < map_height - 2; c.y++) {
      auto type = qsu::type{item_roll_per_level.roll(m_level)};
      if (!type) {
        continue;
      }

      do {
        c.x = cno::random(map_width);
      } while (!map_at(&m_map, c).type->can_walk);

      m_items.add({type, c});
    }
  }

  void move_hero(int dx, int dy) {
    if (game_is_over())
      return;

    const auto &[x, y] = m_player.coord();
    try_move(m_player.mob(), map_coord{x + dx, y + dy});
    tick();
  }

  void use_item() {
    auto pc = m_player.coord();
    if (open_item_at(pc)) {
      return;
    }

    // This should be a different action. If we try to fetch an item from ground
    // and we fail and the item is over the stair, we move to the next level.
    // TODO: fix this.
    if (map_at(&m_map, pc).type->id != gt.id) {
      tick();
      return;
    }

    auto lvl = m_level + 1;
    if (lvl == max_level + 1) {
      g::update_status("You left the dungeon. The world is yours now.");
    } else {
      g::update_status("You stumble in darkness, stairs crumbling behind you!");
    }
    set_level(lvl);
  }

  void repaint() {
    auto pc = m_player.coord();
    unsigned dist = m_light.visible_distance();
    m_map.update_rogueview(pc, dist);
    m_mobs.update_rogueview(pc, dist);
    m_items.update_rogueview(pc, dist);

    hide_mobs_in_fog(&m_mobs);

    m_map.fill_quack();
    m_mobs.fill_quack();
    m_items.fill_quack();

    debug_mobs(&m_mobs);
  }

  void set_level(unsigned l) {
    m_level = l;
    m_player.level_reset(l);
    maze_builder{&m_map}.build_level(l);
    create_items();
    create_enemies();
    repaint();
  }

  void tick() {
    process_actions_with_light();
    m_light.tick();
    repaint();
  }

  void update_animations(float dt) {
    m_mobs.for_each([dt](auto &m) { m.damage_timer -= dt; });
  }

public:
  void process_event(const casein::event &e) {
    m_r.process_event(e);
    m_map.process_event(e);
    m_items.process_event(e);
    m_mobs.process_event(e);

    if (e.type() == casein::CREATE_WINDOW) {
      atlas::load(m_r);
    }
  }

  void reset() {
    set_level(1);
    m_inv = {};
  }

  void use() {
    if (game_is_over())
      return;

    use_item();
    // center at player
  }

  void down() { move_hero(0, 1); }
  void left() { move_hero(-1, 0); }
  void right() { move_hero(1, 0); }
  void up() { move_hero(0, -1); }
};
} // namespace cno
