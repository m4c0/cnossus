export module save;
import fork;
import buoy;

export namespace save {
struct data {
  unsigned version{1};
  unsigned level{};
} d;

void read() {
  buoy::open_for_reading("cnossus", "save.dat")
      .fpeek(frk::assert("CNO"))
      .fpeek(frk::take("DATA", &d))
      .map(frk::end())
      .trace("reading save data")
      .log_error();
}

void write() {
  buoy::open_for_writing("cnossus", "save.dat")
      .fpeek(frk::signature("CNO"))
      .fpeek(frk::chunk("DATA", &d))
      .map(frk::end())
      .trace("writing save data")
      .log_error();
}
} // namespace save
