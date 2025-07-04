#ifndef LECO_TARGET_IOS
#pragma leco app
#endif

import casein;
import map;
import play;
import qsu;
import save;
import quack;

static void shots() { quack::donald::offscreen(800, 600); }

struct init {
  init() {
    using namespace casein;
    handle(KEY_DOWN, K_SPACE, shots);

    qsu::init();
    save::read([] {}, [] {});
    play::redraw();
  }
} i;
