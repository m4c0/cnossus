#include "../casein/build.hpp"
#include "../ecow/ecow.hpp"
#include "../jute/build.hpp"
#include "../pog/build.hpp"
#include "../quack/build.hpp"
#include "../rng/build.hpp"
#include "../silog/build.hpp"
#include "../sires/build.hpp"
#include "../stubby/build.hpp"
#include "../traits/build.hpp"

class atlas : public ecow::seq {
  std::string exe;

  void build_self() const override {
    seq::build_self();

    const auto img = ecow::impl::current_target()->build_path() / name();

    std::cerr << "assembling " << name() << std::endl;
    auto cmd = exe + " " + img.string();
    if (std::system(cmd.c_str()) != 0)
      throw std::runtime_error("Failed to assemble atlas");
  }
  [[nodiscard]] pathset self_objects() const override { return {}; }

public:
  explicit atlas(const std::string &name) : seq{name} {
    auto ab = ecow::unit::create<ecow::tool>("ab");
    ab->add_wsdep("stubby", stubby());
    ab->add_ref(ecow::unit::create<ecow::mod>("spr"));
    ab->add_unit("ab");
    this->add_ref(ab);

    exe = ab->executable().string();
  }
};

int main(int argc, char **argv) {
  using namespace ecow;

  auto spr = unit::create<mod>("spr");

  auto qsu = unit::create<mod>("qsu");
  qsu->add_wsdep("casein", casein());
  qsu->add_wsdep("quack", quack());
  qsu->add_wsdep("silog", silog());
  qsu->add_wsdep("sires", sires());
  qsu->add_wsdep("traits", traits());
  qsu->add_part("id");
  qsu->add_part("layout");
  qsu->add_part("renderer");

  auto inv = unit::create<mod>("inv");
  inv->add_wsdep("pog", pog());

  auto map = unit::create<mod>("map");
  map->add_wsdep("rng", rng());

  auto ecs = unit::create<mod>("ecs");
  ecs->add_wsdep("pog", pog());
  ecs->add_wsdep("rng", rng());
  ecs->add_part("ec");
  ecs->add_part("entities");
  ecs->add_part("loot");
  ecs->add_part("items");
  ecs->add_part("containers");
  ecs->add_part("itemroll");
  ecs->add_part("mobs");
  ecs->add_part("mobroll");

  auto mobs = unit::create<mod>("mobs");
  mobs->add_wsdep("pog", pog());
  mobs->add_wsdep("rng", rng());
  mobs->add_wsdep("silog", silog());

  auto dbg = unit::create<mod>("dbg");
  dbg->add_wsdep("pog", pog());

  auto smol = unit::create<mod>("smol");
  smol->add_wsdep("pog", pog());

  auto m = unit::create<mod>("cno");
  m->add_wsdep("casein", casein());
  m->add_wsdep("jute", jute());
  m->add_wsdep("quack", quack());
  m->add_wsdep("rng", rng());
  m->add_wsdep("silog", silog());
  m->add_wsdep("traits", traits());
  m->add_part("camera");
  m->add_part("objects");
  m->add_part("random");
  m->add_part("entities");
  m->add_part("inventory");
  m->add_part("player");
  m->add_part("light");
  m->add_part("enemy");
  m->add_part("game");

  auto a = unit::create<app>("cnossus");
  a->add_requirement(native);
  a->add_ref(spr);
  a->add_ref(ecs);
  a->add_ref(inv);
  a->add_ref(map);
  a->add_ref(mobs);
  a->add_ref(qsu);
  a->add_ref(dbg);
  a->add_ref(smol);
  a->add_ref(m);
  a->add_unit<atlas>("atlas.img");
  a->add_resource("atlas.img");

  return run_main(a, argc, argv);
}
