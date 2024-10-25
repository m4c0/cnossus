#ifndef LECO_TARGET_IOS
#pragma leco app
#endif

import casein;
import map;
import play;
import qsu;
import quack;
import save;
import vee;
import voo;

void shot(vee::physical_device pd, vee::extent ext) {
  voo::offscreen::colour_buffer m_colour { pd, ext };
  voo::offscreen::depth_buffer m_depth { pd, ext };
  voo::offscreen::host_buffer m_host { pd, ext };
}

struct init {
  init() {
    using namespace quack::donald;

    app_name("cnossus");
    max_quads(map::width * map::height * 2 + 1024);

    clear_colour({ 0, 0, 0, 1 });
    atlas(qsu::atlas);

    save::read([] {}, [] {});

    play::redraw();
  }
} i;
