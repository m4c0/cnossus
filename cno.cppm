export module cno;
import :globals;
import :inventory;
import :itemlist;
import :map;
import :moblist;
import casein;
import jute;
import quack;

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

  void process_actions_with_light() {
    m_mobs.for_each([this](auto &m) {
      auto pc = m_mobs.player()->coord();

      auto tgt = m->act_with_light(pc, m_light);
      const auto *blk = m_map.at(tgt.x, tgt.y);
      if (!blk->can_walk()) {
        if (m->type() == &minotaur) {
          using namespace jute::literals;
          g::update_status("A "_s + blk->name() + " blocks your way");
        }
        return;
      }

      auto *mm = m_mobs.mob_at(tgt);
      if (mm != nullptr && (*mm)->type() != m->type()) {
        attack(*m, *mm);
        return;
      }

      m->coord() = tgt;
    });
    m_mobs.for_each([this](auto &m) {
      if (m->life() <= 0) {
        m_items.add_item({m->type()->random_drop(), m->coord()});
        m = {};
      }
    });
  }

  void attack(const mob &src, auto &tgt) {
    const auto srcn = src.type()->name();
    const auto tgtn = tgt->type()->name();

    int atk_roll = src.type()->dice_roll(2) + src.attack_bonus();
    int def_roll = src.type()->dice_roll(2) + tgt->defense_bonus();
    int margin = atk_roll - def_roll;

    if (margin > 0) {
      margin += src.attack_bonus() - tgt->defense_bonus();
      if (margin < 0)
        return;

      if (tgt->damage_by(margin) <= 0) {
        m_items.add_item({tgt->type()->random_drop(), tgt->coord()});
        tgt = {};
        if (src.type() == &minotaur) {
          g::update_status(srcn + " kill " + tgtn);
        }
      } else if (src.type()->poison() > 0) {
        tgt->poison_by(1 + cno::random(src.type()->poison()));
        if (tgt->type() == &minotaur) {
          g::update_status(srcn + " poisons " + tgtn);
        }
      } else if (src.type() == &minotaur) {
        g::update_status(srcn + " hit " + tgtn);
      } else if (tgt->type() == &minotaur) {
        g::update_status(srcn + " hits " + tgtn);
      }
    } else if (margin == 0) {
      if (src.type()->poison() > 0) {
        tgt->poison_by(1 + cno::random(src.type()->poison()));
        if (tgt->type() == &minotaur) {
          g::update_status(srcn + " poisons " + tgtn);
        }
      } else if (src.type() == &minotaur) {
        g::update_status(srcn + " barely miss " + tgtn);
      } else if (tgt->type() == &minotaur) {
        g::update_status(srcn + " barely misses " + tgtn);
      }
    } else if (src.type() == &minotaur) {
      g::update_status(srcn + " miss " + tgtn);
    } else if (tgt->type() == &minotaur) {
      g::update_status(srcn + " misses " + tgtn);
    }
  }

  [[nodiscard]] bool game_is_over() const { return m_mobs.player() == nullptr; }

  [[nodiscard]] bool use_item() {
    reset_status();

    auto pl = m_mobs.player();
    auto pc = pl->coord();
    if (open_item_at(pc)) {
      pl->update_inventory(m_inv);
      return tick();
    }

    if (m_map.at(pc.x, pc.y) != &gt) {
      return tick();
    }

    auto lvl = m_map.level() + 1;
    if (lvl == 20)
      return true;

    g::update_status("You stumble in darkness, stairs crumbling behind you!");
    set_level(lvl);
    return false;
  }

  void reset_status() {
    // TODO
  }

  void set_level(unsigned l) {
    m_map.set_level(1);
    m_items.create_for_map(&m_map);
    m_mobs.populate_level(&m_map);
  }

  [[nodiscard]] bool tick() {
    process_actions_with_light();
    update_light();
    return game_is_over();
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
};
} // namespace cno

extern "C" void casein_handle(const casein::event &e) {
  static cno::game gg{};
  static constexpr auto map = [] {
    casein::event_map res{};
    res[casein::CREATE_WINDOW] = [](auto) { gg.reset(); };
    return res;
  }();

  gg.process_event(e);
  map.handle(e);
}
