export module inv;
import quack;
import spr;

namespace inv {
bool bag{};
spr::id lights[3]{};

export void init(int level) {
  if (level == 1) {
    bag = false;
    for (auto &l : lights)
      l = spr::nil;
  }
}
export auto draw(quack::mapped_buffers &all) {
  //
  return 0;
}
} // namespace inv
