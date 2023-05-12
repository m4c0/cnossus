export module qsu:idholder;
import :id;
import traits;

export namespace qsu {
template <typename Tp>
concept id_holder = requires(const Tp &x) {
  { x.id } -> traits::same_as<const id &>;
};

// TODO: remove?
[[nodiscard]] constexpr bool operator==(const id_holder auto &a,
                                        const id_holder auto &b) noexcept {
  return a.id == b.id;
}
} // namespace qsu
