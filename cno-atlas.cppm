export module cno:atlas;
import silog;
import sires;
import quack;

namespace cno::atlas {
static constexpr const auto sz = 512;
static constexpr const auto pixies = sz * sz * 4;

void load(quack::renderer &r) {
  r.load_atlas(sz, sz, [](auto *p) {
    sires::open("atlas.img")
        .fmap([p](auto &rdr) { return rdr->read(p, pixies); })
        .map([] { silog::log(silog::info, "Atlas loaded"); })
        .take([](auto err) { silog::log(silog::error, err); });
  });
}
} // namespace cno::atlas
