export module cno;
import :globals;
import :inventory;
import :itemlist;
import :map;
import :moblist;
import casein;
import jute;
import quack;
import silog;

namespace cno {
class game {
  quack::renderer m_r{2};
  map m_map{&m_r};
  item_list m_items{&m_r};
  mob_list m_mobs{&m_r};
  inv::table m_inv{};
  unsigned m_light{};

  bool open_item_at(map_coord c) {
    auto it = m_items.fetch(c);
    if (it == nullptr)
      return false;

    auto nit = it->drop_for_level(m_map.level());
    if (nit != it) {
      m_items.add_item({nit, c});
      return false;
    }

    return m_inv.get_item(nit);
  }

  void try_move(mob *m, map_coord tgt) {
    const auto *blk = m_map.at(tgt.x, tgt.y);
    if (!blk->can_walk()) {
      if (m->is_player()) {
        using namespace jute::literals;
        g::update_status("A "_s + blk->name() + " blocks your way");
      }
      return;
    }

    auto *mm = m_mobs.mob_at(tgt);
    if (mm != nullptr && !m->same_species_as(**mm)) {
      attack(*m, *mm);
      return;
    }

    m->set_coord(tgt);
  }

  void process_actions_with_light() {
    m_mobs.for_each([this](auto &m) {
      if (!m->update_actions())
        return;

      auto pc = m_mobs.player()->coord();
      auto tgt = m->next_move_with_light(pc, m_light);
      if (tgt != m->coord())
        try_move(&*m, tgt);
    });
    m_mobs.for_each([this](auto &m) {
      if (m->life() <= 0) {
        m_items.add_item({m->random_drop(), m->coord()});
        m = {};
      }
    });
  }

  void attack(const mob &src, auto &tgt) {
    const auto srcn = src.name();
    const auto tgtn = tgt->name();

    int atk_roll = src.dice_roll(2) + src.attack_bonus();
    int def_roll = src.dice_roll(2) + tgt->defense_bonus();
    int margin = atk_roll - def_roll;

    if (margin > 0) {
      margin += src.attack_bonus() - tgt->defense_bonus();
      if (margin < 0)
        return;

      if (tgt->damage_by(margin) <= 0) {
        m_items.add_item({tgt->random_drop(), tgt->coord()});
        tgt = {};
        if (src.is_player()) {
          g::update_status("You kill a " + tgtn);
        }
      } else if (src.poison() > 0) {
        tgt->poison_by(1 + cno::random(src.poison()));
        if (tgt->is_player()) {
          g::update_status("A " + srcn + " poisons you");
        }
      } else if (src.is_player()) {
        g::update_status("You hit a " + tgtn);
      } else if (tgt->is_player()) {
        g::update_status("A " + srcn + " hits you");
      }
    } else if (margin == 0) {
      if (src.poison() > 0) {
        tgt->poison_by(1 + cno::random(src.poison()));
        if (src.is_player()) {
          g::update_status("A " + srcn + " poisons you");
        }
      } else if (src.is_player()) {
        g::update_status("You barely miss " + tgtn);
      } else if (tgt->is_player()) {
        g::update_status("A " + srcn + " barely misses you");
      }
    } else if (src.is_player()) {
      g::update_status("You miss a " + tgtn);
    } else if (tgt->is_player()) {
      g::update_status("A " + srcn + " misses you");
    }
  }

  void consume(const item_type *t) {
    reset_status();

    if (!m_inv.consume(t))
      return;

    if (t->life_gain() > 0) {
      m_mobs.player()->recover_health(t->life_gain());
    }
    if (t->light_provided() > 0) {
      m_light += t->light_provided();
    }
    tick();
  }

  [[nodiscard]] bool game_is_over() const {
    return m_mobs.player() == nullptr || m_map.level() == 20;
  }

  void move_hero(int dx, int dy) {
    if (game_is_over())
      return;

    reset_status();

    const auto &[x, y] = m_mobs.player()->coord();
    try_move(m_mobs.player(), map_coord{x + dx, y + dy});
    tick();
  }

  void use_item() {
    reset_status();

    auto pl = m_mobs.player();
    auto pc = pl->coord();
    if (open_item_at(pc)) {
      pl->update_inventory(m_inv);
      tick();
      return;
    }

    if (m_map.at(pc.x, pc.y) != &gt) {
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

  void repaint() {
    auto pc = m_mobs.player()->coord();
    unsigned dist = m_light < 1 ? 2 : 5;
    m_map.fill_quack(pc, dist);
    m_mobs.fill_quack(pc, dist);
    m_items.fill_quack(pc, dist);
  }

  void reset_status() {
    // TODO
  }

  void set_level(unsigned l) {
    m_map.set_level(l);
    m_items.create_for_map(&m_map);
    m_mobs.populate_level(&m_map);
    repaint();
  }

  void tick() {
    process_actions_with_light();
    update_light();
    repaint();
  }

  void update_light() {
    if (m_light == 0)
      return;

    m_light--;
    if (m_light == 0) {
      g::update_status("Your light runs out");
    }
  }

  void update_animations(float dt) { m_mobs.update_animations(dt); }

public:
  void process_event(const casein::event &e) {
    m_r.process_event(e);
    m_map.process_event(e);
    m_items.process_event(e);
    m_mobs.process_event(e);
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

void cno::g::update_status(const char *msg) { silog::log(silog::info, msg); }

extern "C" void casein_handle(const casein::event &e) {
  static cno::game gg{};
  static constexpr const auto k_map = [] {
    casein::key_map res{};
    res[casein::K_DOWN] = [](auto) { gg.down(); };
    res[casein::K_LEFT] = [](auto) { gg.left(); };
    res[casein::K_RIGHT] = [](auto) { gg.right(); };
    res[casein::K_UP] = [](auto) { gg.up(); };
    res[casein::K_SPACE] = [](auto) { gg.use(); };
    return res;
  }();
  static constexpr const auto map = [] {
    casein::event_map res{};
    res[casein::CREATE_WINDOW] = [](auto) { gg.reset(); };
    res[casein::KEY_DOWN] = [](auto e) { k_map.handle(e); };
    return res;
  }();

  gg.process_event(e);
  map.handle(e);
}
