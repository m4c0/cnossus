export module cno;
import :itemlist;
import :map;
import casein;
import quack;

namespace cno {
class game {
  quack::renderer m_r{2};
  map m_map{&m_r};
  item_list m_items{&m_r};

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
