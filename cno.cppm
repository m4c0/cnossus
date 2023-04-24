export module cno;
import casein;
import quack;

namespace cno {
class game {
  quack::renderer m_r{1};

public:
  void process_event(const casein::event &e) { m_r.process_event(e); }
};
} // namespace cno

extern "C" void casein_handle(const casein::event &e) {
  static cno::game gg{};
  static constexpr auto map = [] {
    casein::event_map res{};
    return res;
  }();

  gg.process_event(e);
}
