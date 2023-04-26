export module cno:globals;
import :objects;
import jute;

namespace cno::g {
void update_status(const char *msg) {
  // TODO: implement
}
template <unsigned N> inline void update_status(const jute::twine<N> &t) {
  update_status(t.cstr().data());
};
} // namespace cno::g
