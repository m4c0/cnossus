export module qsu:sprite;
import :coord;
import :idholder;
import :type;
import :vis;

export namespace qsu {
template <id_holder Tp> struct sprite {
  type<Tp> type{};
  coord coord{};
  vis vis{};
};
} // namespace qsu
