#pragma leco add_impl sfx_impl
export module sfx;

namespace sfx {
export constexpr const auto max_volume = 0.1f;
export extern float main_volume;
export void menu_move();
export void menu_select();
} // namespace sfx
