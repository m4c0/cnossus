#include "../casein/build.hpp"
#include "../ecow/ecow.hpp"
#include "../jute/build.hpp"
#include "../quack/build.hpp"

int main(int argc, char **argv) {
  using namespace ecow;

  auto m = unit::create<mod>("cno");
  m->add_wsdep("casein", casein());
  m->add_wsdep("jute", jute());
  m->add_wsdep("quack", quack());
  m->add_part("blocktype");

  auto a = unit::create<app>("cnossus");
  a->add_requirement(native);
  a->add_ref(m);

  return run_main(a, argc, argv);
}
