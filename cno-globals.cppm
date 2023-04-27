export module cno:globals;
import :objects;
import jute;

namespace cno {
static constexpr const auto map_width = 30;
static constexpr const auto map_height = 20;

static constexpr const auto max_level = 20;
} // namespace cno

namespace cno::g {
void update_status(const char *msg) {
  // TODO: implement
}
template <unsigned N> inline void update_status(const jute::twine<N> &t) {
  update_status(t.cstr().data());
};
} // namespace cno::g
