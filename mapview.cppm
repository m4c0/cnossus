#pragma leco app
export module mapview;

import casein;
import spr;

static constexpr const auto map_width = 30;
static constexpr const auto map_height = 20;

spr::id map[map_height][map_width]{};

struct init {
  init() {}
} i;

