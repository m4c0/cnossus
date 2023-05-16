export module cno:globals;
import :objects;
import jute;

namespace cno {
static constexpr const auto max_level = 20;
} // namespace cno

namespace cno::g {
void update_status(const char *msg);
template <unsigned N> inline void update_status(const jute::twine<N> &t) {
  update_status(t.cstr().data());
};
} // namespace cno::g
