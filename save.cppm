export module save;
import buoy;
import enemies;
import fork;
import loot;
import map;
import player;
import silog;
import spr;

export namespace save {
struct data {
  unsigned version{1};
  unsigned level{};
} d;

[[nodiscard]] auto read() {
  d = {};
  return buoy::open_for_reading("cnossus", "save.dat")
      .fpeek(frk::assert("CNO"))
      .fpeek(frk::take("DATA", &d))
      .fpeek(frk::take("PLAY", &player::d))
      .fpeek(frk::take("FOES", &enemies::d))
      .fpeek(frk::take("LOOT", &loot::d))
      .fpeek(frk::take("MAPA", &map::d))
      .map(frk::end())
      .map([] { silog::log(silog::info, "Game loaded"); })
      .trace("reading save data");
}

void write() {
  buoy::open_for_writing("cnossus", "save.dat")
      .fpeek(frk::signature("CNO"))
      .fpeek(frk::chunk("DATA", &d))
      .fpeek(frk::chunk("PLAY", &player::d))
      .fpeek(frk::chunk("FOES", &enemies::d))
      .fpeek(frk::chunk("LOOT", &loot::d))
      .fpeek(frk::chunk("MAPA", &map::d))
      .map(frk::end())
      .map([] { silog::log(silog::info, "Game saved"); })
      .trace("writing save data")
      .log_error();
}

void reset() {
  d = {};
  write();
}
} // namespace save
