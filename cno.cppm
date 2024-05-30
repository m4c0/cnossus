#pragma leco app

export module cno;
import :game;
import casein;
import smol;

// static cno::game gg{};
static smol::game gg{};
static void reset() { gg.reset(); }
static void down() { gg.down(); }
static void left() { gg.left(); }
static void right() { gg.right(); }
static void up() { gg.up(); }
static void use() { gg.use(); }

static struct init {
  init() {
    using namespace casein;

    handle(CREATE_WINDOW, reset);
    handle(KEY_DOWN, K_DOWN, down);
    handle(KEY_DOWN, K_LEFT, down);
    handle(KEY_DOWN, K_RIGHT, down);
    handle(KEY_DOWN, K_UP, down);
    handle(KEY_DOWN, K_SPACE, use);
  }
} i;
