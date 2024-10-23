export module save;
import buoy;
import enemies;
import fork;
import hai;
import inv;
import jute;
import light;
import loot;
import map;
import missingno;
import player;
import silog;
import spr;
import traits;
import yoyo;

using namespace traits::ints;

export namespace save {
struct data {
  unsigned version{1};
  unsigned level{};
} d;

void read(hai::fn<void> ok, hai::fn<void> err) {
  d = {};
  buoy::on_failure = [err](const char * msg) mutable {
    silog::log(silog::warning, "Error loading save data: %s", msg);
    err();
  };
  buoy::read("cnossus", "save.dat", [ok, err](auto & data) mutable {
    auto ptr = reinterpret_cast<const uint8_t *>(data.begin());
    mno::req { yoyo::memreader { ptr, data.size() } }
      .fpeek(frk::assert("CNO"))
      .fpeek(frk::take("DATA", &d))
      .fpeek(frk::take("PLAY", &player::d))
      .fpeek(frk::take("FOES", &enemies::d))
      .fpeek(frk::take("LOOT", &loot::d))
      .fpeek(frk::take("LGHT", &light::d))
      .fpeek(frk::take("PACK", &inv::d))
      .fpeek(frk::take("MAPA", &map::d))
      .map(frk::end())
      .map([] { silog::log(silog::info, "Game loaded"); })
      .map(ok)
      .take([&](auto msg) {
        silog::log(silog::warning, "Error loading save data: %.*s", static_cast<unsigned>(msg.size()), msg.data());
        err();
      });
  });
}

void write() {
  hai::array<uint8_t> buffer { 20000 };
  mno::req { yoyo::memwriter { buffer } }
      .fpeek(frk::signature("CNO"))
      .fpeek(frk::chunk("DATA", &d))
      .fpeek(frk::chunk("PLAY", &player::d))
      .fpeek(frk::chunk("FOES", &enemies::d))
      .fpeek(frk::chunk("LOOT", &loot::d))
      .fpeek(frk::chunk("LGHT", &light::d))
      .fpeek(frk::chunk("PACK", &inv::d))
      .fpeek(frk::chunk("MAPA", &map::d))
      .map([&](auto & w) {
        auto buf = reinterpret_cast<const char *>(buffer.begin());
        jute::heap data { jute::view { buf, w.raw_size() } };
        buoy::write("cnossus", "save.dat", data); 
        silog::log(silog::info, "Game saved");
      })
      .trace("writing save data")
      .log_error();
}

void clear() {
  d = {};
  hai::array<uint8_t> buffer { 1024 };
  mno::req { yoyo::memwriter { buffer } }
      .fpeek(frk::signature("CNO"))
      .map([&](auto & w) {
        auto buf = reinterpret_cast<const char *>(buffer.begin());
        jute::heap data { jute::view { buf, w.raw_size() } };
        buoy::write("cnossus", "save.dat", data); 
        silog::log(silog::info, "Game cleared");
      })
      .trace("writing save data")
      .log_error();
}

void reset() {
  d = {};
  write();
}
} // namespace save
