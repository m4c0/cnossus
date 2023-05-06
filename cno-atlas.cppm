export module cno:atlas;
import quack;

namespace cno::atlas {
constexpr const auto width = 16;
constexpr const auto height = 16;
using pixel = quack::u8_rgba;

class image {
public:
  void operator()(pixel *data) {}
};

void load(quack::renderer &r) { r.load_atlas(width, height, image{}); }
} // namespace cno::atlas
