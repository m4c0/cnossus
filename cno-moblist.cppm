export module cno:moblist;
import :globals;
import :mob;
import :mobtype;
import quack;

namespace cno {
static constexpr const auto max_mobs_per_level = map_height * 2;
static constexpr const auto max_mob_roll = 5;

static constexpr const struct {
  const mob_type *mobs[max_level][max_mob_roll];
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

class mob_list : quack::instance_layout<mob, max_mobs_per_level> {
  void fill_quack() {
    fill_pos([](const mob &i) {
      const auto &c = i.coord();
      return quack::rect{static_cast<float>(c.x), static_cast<float>(c.y), 1,
                         1};
    });
    fill_colour([](const mob &i) {
      float a = i.type() == nullptr ? 0.0 : 1.0;
      return quack::colour{0.3, 0.3, 0.3, a};
    });
  }

  void resize(unsigned w, unsigned h) override {
    batch()->resize(map_width, map_height, w, h);
  }

public:
  explicit mob_list(quack::renderer *r) : instance_layout{r} {}
};
} // namespace cno
