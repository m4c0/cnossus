export module cno:moblist;
import :enemy;
import :globals;
import :map;
import :mob;
import :mobtype;
import :player;
import hai;
import quack;

namespace cno {
static constexpr const auto max_mobs_per_level = map_height * 2;
static constexpr const auto max_mob_roll = 5;

static constexpr const struct {
  const mob_type *mobs[max_level + 1][max_mob_roll];
} mob_roll_per_level = {{
    {&snake, &boar, &cerberus},     {&centipede, &boar, &harpy},
    {&snake, &cerberus, &rat},      {&centipede, &harpy, &rat},
    {&snake, &cerberus, &boar},     {&centipede, &harpy, &boar},
    {&snake, &cerberus, &rat},      {&spider, &rat, &harpy},
    {&scorpion, &boar, &croc},      {&spider, &boar, &harpy},
    {&scorpion, &croc, &rat, &rat}, {&spider, &rat, &cerberus, &harpy},
    {&scorpion, &croc, &boar},      {&spider, &boar, &rat, &harpy},
    {&manticore, &griffin, &croc},  {&griffin, &bull, &spider},
    {&sphinx, &manticore, &bull},   {&sphinx, &chimera, &bull},
    {&manticore, &chimera, &bull},  {&drakon, &griffin, &bull},
}};

// TODO: consider how we can do this without hai/polymorphism
class mob_list : quack::instance_layout<hai::uptr<mob>, max_mobs_per_level> {
  void resize(unsigned w, unsigned h) override {
    batch()->resize(map_width, map_height, w, h);
  }

  void update_player(unsigned lvl) {
    auto px = (lvl % 2 == 1) ? 1U : map_width - 2U;
    map_coord pc{px, 1};

    if (lvl == 1) {
      at(0) = hai::uptr<mob>{new cno::player(pc)};
    } else {
      auto p = player();
      p->coord() = pc;
      p->random_buff();
    }
  }

  void create_enemies(const map *m) {
    const auto &mob_roll = mob_roll_per_level.mobs[m->level()];
    map_coord c{};
    for (c.y = 1; c.y < map_height - 1; c.y++) {
      const mob_type *t = mob_roll[cno::random(max_mobs_per_level)];
      if (t == nullptr) {
        at(c.y) = {};
        continue;
      }

      do {
        c.x = cno::random(map_width);
      } while (!m->at(c.x, c.y)->can_walk());

      at(c.y) = hai::uptr<mob>{new enemy(t, c, m->level())};
    }
  }

public:
  explicit mob_list(quack::renderer *r) : instance_layout{r} {}

  using instance_layout::process_event;

  [[nodiscard]] cno::player *player() noexcept {
    return static_cast<cno::player *>(&*at(0));
  }
  [[nodiscard]] const cno::player *player() const noexcept {
    return static_cast<const cno::player *>(&*at(0));
  }

  void fill_quack() {
    fill_pos([](auto &i) {
      if (!i)
        return quack::rect{};

      const auto &c = i->coord();
      return quack::rect{static_cast<float>(c.x), static_cast<float>(c.y), 1,
                         1};
    });
    fill_colour([](auto &i) {
      if (!i)
        return quack::colour{};

      auto c = i->type()->character();
      auto r = static_cast<float>(c % 16) / 16.0f;
      auto b = static_cast<float>(c / 16) / 16.0f;

      float a = i->type() == nullptr ? 0.0 : 1.0;

      return quack::colour{r, 0, b, a};
    });
  }

  void populate_level(const map *m) {
    update_player(m->level());
    create_enemies(m);
    fill_quack();
  }

  [[nodiscard]] constexpr hai::uptr<mob> *mob_at(map_coord c) {
    for (auto &m : data()) {
      if (m && m->coord() == c)
        return &m;
    }
    return nullptr;
  }

  void update_animations(float dt) {
    for (auto &m : data()) {
      if (m)
        m->update_animations(dt);
    }
  }

  void for_each(auto &&fn) {
    for (auto &m : data()) {
      if (!m)
        continue;
      fn(m);
    }
  }
};
} // namespace cno