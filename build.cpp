#include "../casein/build.hpp"
#include "../ecow/ecow.hpp"
#include "../hai/build.hpp"
#include "../jute/build.hpp"
#include "../quack/build.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto m = unit::create<mod>("cno");
  m->add_wsdep("casein", casein());
  m->add_wsdep("hai", hai());
  m->add_wsdep("jute", jute());
  m->add_wsdep("quack", quack());
  m->add_part("objects");
  m->add_part("globals");
  m->add_part("random");
  m->add_part("blocktype");
  m->add_part("map");
  m->add_part("itemtype");
  m->add_part("item");
  m->add_part("inventory");
  m->add_part("itemlist");
  m->add_part("mobtype");
  m->add_part("mob");
  m->add_part("player");
  m->add_part("enemy");
  m->add_part("moblist");

  auto a = unit::create<app>("cnossus");
  a->add_requirement(native);
  a->add_ref(m);

  return run_main(a, argc, argv);
}
