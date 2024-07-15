export module save;
import fork;
import buoy;

export namespace save {
struct player_data {
  static constexpr const auto base_attack = 10;
  static constexpr const auto base_defense = 10;
  static constexpr const auto base_life = 10;

  int life{base_life};
  int max_life{base_life};
  int poison{};
  int attack{base_attack};
  int defense{base_defense};
};

struct data {
  unsigned version{1};
  unsigned level{};
  player_data player{};
} d;

[[nodiscard]] auto read() {
  d = {};
  return buoy::open_for_reading("cnossus", "save.dat")
      .fpeek(frk::assert("CNO"))
      .fpeek(frk::take("DATA", &d))
      .map(frk::end())
      .trace("reading save data");
}

void write() {
  buoy::open_for_writing("cnossus", "save.dat")
      .fpeek(frk::signature("CNO"))
      .fpeek(frk::chunk("DATA", &d))
      .map(frk::end())
      .trace("writing save data")
      .log_error();
}

void reset() {
  d = {};
}
} // namespace save
