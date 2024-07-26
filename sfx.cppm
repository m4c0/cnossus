#pragma leco add_impl sfx_impl
export module sfx;

namespace sfx {
export constexpr const auto max_volume = 0.1f;
export extern float main_volume;

export void menu_move();
export void menu_select();

export void fail();

export void next_level();

export void eat();
export void light();
export void pick();
export void walk();
export void break_jar();
} // namespace sfx
