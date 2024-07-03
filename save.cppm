export module save;
import buoy;

export namespace save {
struct data {
  unsigned level{};
} d;

void read() {
  buoy::open_for_reading("cnossus", "save.dat")
      .fmap([](auto &r) { return r.template read<data>(); })
      .map([](auto dd) { d = dd; })
      .trace("reading save data")
      .log_error();
}

void write() {
  buoy::open_for_writing("cnossus", "save.dat")
      .fmap([](auto &w) { return w.write(d); })
      .trace("writing save data")
      .log_error();
}
} // namespace save
