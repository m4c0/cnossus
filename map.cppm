export module map;
import rng;
import spr;
import quack;

export namespace map {
constexpr const auto width = 30;
constexpr const auto height = 20;

spr::id data[height][width]{};

unsigned draw(quack::mapped_buffers &all) {
  unsigned count{};
  for (auto y = 0; y < map::height; y++) {
    for (auto x = 0; x < map::width; x++) {
      auto b = data[y][x];
      count += spr::blit(b, x, y, all);
    }
  }
  return count;
}
} // namespace map
