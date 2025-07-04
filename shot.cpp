#ifndef LECO_TARGET_IOS
#pragma leco app
#endif

import casein;
import map;
import play;
import qsu;
import save;

struct init {
  init() {
    using namespace casein;
    handle(KEY_DOWN, K_SPACE, qsu::screenshot);

    qsu::init();
    save::read([] {}, [] {});
    play::redraw();
  }
} i;
