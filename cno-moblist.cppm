export module cno:moblist;
import :mob;
import quack;

namespace cno {
class mob_list : quack::instance_layout<mob, max_mobs_per_level> {
  void resize(unsigned w, unsigned h) override {
    batch()->resize(map_width, map_height, w, h);
  }

public:
  explicit mob_list(quack::renderer *r) : instance_layout{r} {}

  using instance_layout::at;
  using instance_layout::process_event;

  void fill_quack(map_coord pc, unsigned d) {
    const auto &[px, py] = pc;

    fill_pos([](auto &i) {
      if (!i)
        return quack::rect{};

      const auto &c = i.coord();
      return quack::rect{static_cast<float>(c.x), static_cast<float>(c.y), 1,
                         1};
    });
    fill_colour([px, py, d](auto &i) {
      if (!i)
        return quack::colour{};

      const auto &[x, y] = i.coord();
      auto dx = px - x;
      auto dy = py - y;

      auto c = i.type()->character;
      auto r = static_cast<float>(c % 16) / 16.0f;
      auto b = static_cast<float>(c / 16) / 16.0f;
      auto a = (dx * dx + dy * dy) <= d ? 1.0f : 0.3f;
      return quack::colour{r, 0, b, a};
    });
  }

  [[nodiscard]] constexpr mob *mob_at(map_coord c) {
    for (auto &m : data()) {
      if (m && m.coord() == c)
        return &m;
    }
    return nullptr;
  }

  void for_each(auto &&fn) {
    for (auto &m : data()) {
      if (m)
        fn(m);
    }
  }
};
} // namespace cno
