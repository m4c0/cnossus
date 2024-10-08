export module save;
import buoy;
import enemies;
import fork;
import inv;
import light;
import loot;
import map;
import player;
import silog;
import spr;
import missingno;

export namespace save {
struct data {
  unsigned version{1};
  unsigned level{};
} d;

[[nodiscard]] auto read() {
  d = {};
#ifdef LECO_TARGET_WASM
  return mno::req<void>::failed("eh");
#else
  return buoy::open_for_reading("cnossus", "save.dat")
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
      .trace("reading save data");
#endif
}

void write() {
#ifndef LECO_TARGET_WASM
  buoy::open_for_writing("cnossus", "save.dat")
      .fpeek(frk::signature("CNO"))
      .fpeek(frk::chunk("DATA", &d))
      .fpeek(frk::chunk("PLAY", &player::d))
      .fpeek(frk::chunk("FOES", &enemies::d))
      .fpeek(frk::chunk("LOOT", &loot::d))
      .fpeek(frk::chunk("LGHT", &light::d))
      .fpeek(frk::chunk("PACK", &inv::d))
      .fpeek(frk::chunk("MAPA", &map::d))
      .map(frk::end())
      .map([] { silog::log(silog::info, "Game saved"); })
      .trace("writing save data")
      .log_error();
#endif
}

void clear() {
  d = {};
#ifndef LECO_TARGET_WASM
  buoy::open_for_writing("cnossus", "save.dat")
      .fpeek(frk::signature("CNO"))
      .map(frk::end())
      .map([] { silog::log(silog::info, "Save game cleared"); })
      .trace("clearing save data")
      .log_error();
#endif
}

void reset() {
  d = {};
  write();
}
} // namespace save
