#include "../casein/build.hpp"
#include "../ecow/ecow.hpp"
#include "../jute/build.hpp"
#include "../quack/build.hpp"
#include "../silog/build.hpp"
#include "../traits/build.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto qsu = unit::create<mod>("qsu");
  qsu->add_wsdep("quack", quack());
  qsu->add_wsdep("traits", traits());
  qsu->add_part("coord");
  qsu->add_part("id");
  qsu->add_part("idholder");
  qsu->add_part("vis");

  auto m = unit::create<mod>("cno");
  m->add_wsdep("casein", casein());
  m->add_wsdep("jute", jute());
  m->add_wsdep("quack", quack());
  m->add_wsdep("silog", silog());
  m->add_wsdep("traits", traits());
  m->add_part("sprite");
  m->add_part("objects");
  m->add_part("globals");
  m->add_part("random");
  m->add_part("entities");
  m->add_part("blocktype");
  m->add_part("itemtype");
  m->add_part("mobtype");
  m->add_part("map");
  m->add_part("labyrinth");
  m->add_part("inventory");
  m->add_part("itemlist");
  m->add_part("moblist");
  m->add_part("player");
  m->add_part("enemy");
  m->add_part("atlas");
  m->add_part("game");

  auto a = unit::create<app>("cnossus");
  a->add_requirement(native);
  a->add_ref(qsu);
  a->add_ref(m);

  return run_main(a, argc, argv);
}
