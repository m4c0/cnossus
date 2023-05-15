export module qsu:renderer;
import casein;
import quack;
import silog;
import sires;

namespace qsu {
using q_renderer = quack::renderer;

export class renderer : public q_renderer {
public:
  using q_renderer::q_renderer;

  void process_event(const casein::event &e) {
    q_renderer::process_event(e);

    if (e.type() == casein::CREATE_WINDOW) {
      // TODO: make this a parameter
      constexpr const auto sz = 512;
      load_atlas(sz, sz, [](auto *p) {
        sires::open("atlas.img")
            .fmap([p](auto &rdr) { return rdr->read(p, sz * sz * 4); })
            .map([] { silog::log(silog::info, "Atlas loaded"); })
            .take([](auto err) { silog::log(silog::error, err); });
      });
    }
  }
};
} // namespace qsu
