export module cno;
import :inventory;
import :itemlist;
import :map;
import casein;
import quack;

namespace cno {
class game {
  quack::renderer m_r{2};
  map m_map{&m_r};
  item_list m_items{&m_r};
  inv::table m_inv{};

  result<bool> open_item_at(map_coord c) {
    auto it = m_items.fetch(c);
    if (it == nullptr)
      return {"", false};

    auto nit = it->drop_for_level(m_map.level());
    if (nit.value() != it) {
      m_items.add_item({nit.value(), c});
      return {nit.message(), false};
    }

    return m_inv.get_item(nit.value());
  }

public:
  void process_event(const casein::event &e) {
    m_r.process_event(e);
    m_map.process_event(e);
    m_items.process_event(e);
  }

  void create_window() {
    m_map.set_level(1);
    m_items.create_for_map(&m_map);
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
