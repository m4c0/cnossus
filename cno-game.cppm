export module cno:game;
import :atlas;
import :enemy;
import :inventory;
import :itemlist;
import :map;
import :moblist;
import :player;
import casein;
import jute;
import quack;

namespace cno {
class game {
  quack::renderer m_r{3};
  map m_map{&m_r};
  item_list m_items{&m_r};
  mob_list m_mobs{&m_r};
  inv::table m_inv{};
  player m_player{&m_mobs.at(0)};
  unsigned m_light{};

  void create_enemies() {
    map_coord c{};
    for (c.y = 1; c.y < map_height - 1; c.y++) {
      const mob_type *t = mob_roll_per_level.roll(m_map.level());
      if (t == nullptr) {
        m_mobs.at(c.y) = {};
        continue;
      }

      do {
        c.x = cno::random(map_width);
      } while (!m_map.at(c.x, c.y)->can_walk);

      auto &mm = m_mobs.at(c.y) = {sprite{t}, c};
      enemy{&mm}.reset_level(m_map.level());
    }
  }

  [[nodiscard]] bool open_item_at(map_coord c) {
    auto it = m_items.fetch(c);
    if (!it)
      return false;

    auto drops = it->drops;
    auto nit = (drops == nullptr) ? it : sprite{drops->roll(m_map.level())};
    if (!nit) {
      using namespace jute::literals;
      g::update_status("The "_s + it->name + " crumbled to dust");
      return false;
    }

    if (nit != it) {
      g::update_status("Something fells on the ground");
      m_items.add({nit, c});
      return false;
    }

    if (m_inv.get_item(nit)) {
      return true;
    }

    // Add the item back since we can't take it
    m_items.add({it, c});
    return false;
  }

  void try_move(mob *m, map_coord tgt) {
    auto blk = m_map.at(tgt.x, tgt.y);
    if (!blk->can_walk) {
      if (is_player(*m)) {
        using namespace jute::literals;
        g::update_status("A "_s + blk->name + " blocks your way");
      }
      return;
    }

    auto *mm = m_mobs.mob_at(tgt);
    if (mm != nullptr && (m->type != mm->type)) {
      attack(*m, *mm);
      return;
    }

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
        auto drop = sprite{tgt.type->drops.roll()};
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

  void consume(sprite<item_type> t) {
    reset_status();

    if (!m_inv.consume(t))
      return;

    if (t->life_gain > 0) {
      m_player.recover_health(t->life_gain);
    }
    if (t->light_provided > 0) {
      m_light += t->light_provided;
    }
    tick();
  }

  [[nodiscard]] bool game_is_over() const {
    return m_player.is_dead() || m_map.level() == 20;
  }

  void create_items() {
    m_items.reset_grid();

    map_coord c{};
    for (c.y = 1; c.y < map_height - 2; c.y++) {
      auto type = sprite{item_roll_per_level.roll(m_map.level())};
      if (!type) {
        continue;
      }

      do {
        c.x = cno::random(map_width);
      } while (!m_map.at(c.x, c.y)->can_walk);

      m_items.add({type, c});
    }
  }

  void move_hero(int dx, int dy) {
    if (game_is_over())
      return;

    reset_status();

    const auto &[x, y] = m_player.coord();
    try_move(m_player.mob(), map_coord{x + dx, y + dy});
    tick();
  }

  void use_item() {
    reset_status();

    auto pc = m_player.coord();
    if (open_item_at(pc)) {
      m_player.update_inventory(m_inv);
      tick();
      return;
    }

    if (m_map.at(pc.x, pc.y) != sprite{&gt}) {
      tick();
      return;
    }

    auto lvl = m_map.level() + 1;
    if (lvl == max_level + 1) {
      g::update_status("You left the dungeon. The world is yours now.");
    } else {
      g::update_status("You stumble in darkness, stairs crumbling behind you!");
    }
    set_level(lvl);
  }

  void repaint(map_coord pc) {
    unsigned dist = m_light < 1 ? 2 : 5;
    m_map.fill_quack(pc, dist);
    m_mobs.fill_quack(pc, dist);
    m_items.fill_quack(pc, dist);
  }

  void reset_status() {
    // TODO
  }

  void set_level(unsigned l) {
    m_player.level_reset(l);
    m_map.set_level(l);
    create_items();
    create_enemies();
    repaint(m_player.coord());
  }

  void tick() {
    auto pc = m_player.coord();

    process_actions_with_light();
    update_light();
    repaint(pc);
  }

  void update_light() {
    if (m_light == 0)
      return;

    m_light--;
    if (m_light == 0) {
      g::update_status("Your light runs out");
    }
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
    reset_status();
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
