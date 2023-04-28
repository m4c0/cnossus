export module cno;
import :inventory;
import :itemlist;
import :map;
import :moblist;
import casein;
import quack;

namespace cno {
class game {
  quack::renderer m_r{2};
  map m_map{&m_r};
  item_list m_items{&m_r};
  mob_list m_mobs{&m_r};
  inv::table m_inv{};

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
  void process_actions_with_light(unsigned l) {
    m_mobs.for_each([this, l](auto &m) {
      m->process_actions_with_light(l);
      if (m->life() <= 0) {
        m_items.add_item({m->type()->random_drop(), m->coord()});
        m = {};
      }
    });
  }

public:
  void process_event(const casein::event &e) {
    m_r.process_event(e);
    m_map.process_event(e);
    m_items.process_event(e);
    m_mobs.process_event(e);
  }

  void create_window() {
    m_map.set_level(1);
    m_items.create_for_map(&m_map);
    m_mobs.populate_level(&m_map);
  }
};
} // namespace cno

extern "C" void casein_handle(const casein::event &e) {
  static cno::game gg{};
  static constexpr auto map = [] {
    casein::event_map res{};
    res[casein::CREATE_WINDOW] = [](auto) { gg.create_window(); };
    return res;
  }();

  gg.process_event(e);
  map.handle(e);
}
