export module save;
import buoy;

export namespace save {
struct data {
  unsigned level{};
};

data read() {
  return buoy::open_for_reading("cnossus", "save.dat")
      .fmap([](auto &r) { return r.template read<data>(); })
      .trace("reading save data")
      .log_error();
}

void write(data d) {
  buoy::open_for_writing("cnossus", "save.dat")
      .fmap([d](auto &w) { return w.write(d); })
      .trace("writing save data")
      .log_error();
}
} // namespace save
