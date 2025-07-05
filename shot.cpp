#ifndef LECO_TARGET_IOS
#pragma leco app
#endif

import play;
import qsu;
import save;

struct init {
  init() {
    // TODO: fix this
    // using namespace casein;
    // handle(KEY_DOWN, K_SPACE, qsu::screenshot);

    qsu::init();
    save::read([] {}, [] {});
    play::redraw();
  }
} i;
