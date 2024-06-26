#pragma leco app

import casein;
import quack;
import qsu;
import silog;
import spr;
import stubby;
import traits;
import voo;

using namespace traits::ints;

static constexpr const auto cols = 16;
static constexpr const auto rows = 16;

static constexpr const unsigned tile = 32;
static constexpr const unsigned image_w = tile * cols;
static constexpr const unsigned image_h = tile * rows;
static constexpr const unsigned quad_count = 1 + (rows * cols);

static unsigned g_cursor_x{};
static unsigned g_cursor_y{};
static bool g_cursor_hl{};
static uint32_t g_pixies[image_h][image_w]{};

static void update_atlas(voo::h2l_image *img) {
  voo::mapmem m{img->host_memory()};
  auto *buf = static_cast<uint32_t *>(*m);
  for (auto y = 0; y < image_h; y++) {
    for (auto x = 0; x < image_w; x++, buf++) {
      *buf = g_pixies[y][x];
    }
  }
}

static quack::donald::atlas_t *bitmap(voo::device_and_queue *dq) {
  return new quack::donald::atlas_t{dq->queue(), &update_atlas,
                                    dq->physical_device(), image_w, image_h};
}

static void draw() {
  for (auto y = 0, id = 0; y < rows; y++) {
    for (auto x = 0; x < cols; x++, id++) {
      qsu::blit(static_cast<spr::id>(id), x, y);
    }
  }
  if (g_cursor_hl) {
    qsu::guard::colour c{{1, 0, 0, 1}};
    qsu::guard::multiplier m{{1, 1, 1, 0}};
    qsu::blit(spr::cursor, g_cursor_x, g_cursor_y);
  }
}

void refresh_atlas() { quack::donald::atlas(bitmap); }
void refresh_batch() {
  quack::donald::data([](auto all) { return qsu::draw(all, draw); });
}

static void flip_cursor() {
  g_cursor_hl = !g_cursor_hl;
  refresh_batch();
}

static void down() {
  if (g_cursor_y >= rows - 1)
    return;

  g_cursor_y++;
  refresh_batch();
}
static void up() {
  if (g_cursor_y == 0)
    return;

  g_cursor_y--;
  refresh_batch();
}
static void left() {
  if (g_cursor_x == 0)
    return;

  g_cursor_x--;
  refresh_batch();
}
static void right() {
  if (g_cursor_x >= cols - 1)
    return;

  g_cursor_x++;
  refresh_batch();
}

static void save() {
  // TODO: improve safety with a hai::array<pixel> and X/Y accessors
  auto *pix = reinterpret_cast<stbi::pixel *>(g_pixies);
  stbi::write_rgba_unsafe("atlas.png", image_w, image_h, pix);
  silog::log(silog::info, "Atlas saved");
}

static constexpr bool sane_image_width(const stbi::image &img) {
  return img.width <= image_w;
}
static constexpr bool sane_image_height(const stbi::image &img) {
  return img.height <= image_h;
}
static constexpr bool sane_num_channels(const stbi::image &img) {
  return img.num_channels == 4;
}

static void enter_sprite_list() {
  using namespace casein;
  handle(KEY_DOWN, K_DOWN, down);
  handle(KEY_DOWN, K_UP, up);
  handle(KEY_DOWN, K_LEFT, left);
  handle(KEY_DOWN, K_RIGHT, right);
  handle(KEY_DOWN, K_ENTER, save);
}

struct init {
  init() {
    using namespace casein;

    enter_sprite_list();
    handle(TIMER, &flip_cursor);

    stbi::load("atlas.png")
        .assert(sane_image_width, "image is wider than buffer")
        .assert(sane_image_height, "image is taller than buffer")
        .assert(sane_num_channels, "image is not RGBA")
        .map([](auto &&img) {
          auto *d = reinterpret_cast<uint32_t *>(*img.data);
          for (auto y = 0; y < img.height; y++) {
            for (auto x = 0; x < img.width; x++) {
              g_pixies[y][x] = *d++;
            }
          }
        })
        .trace("loading atlas")
        .log_error();

    quack::upc upc{};
    upc.grid_size = {cols, rows};
    upc.grid_pos = upc.grid_size * 0.5;

    using namespace quack::donald;
    app_name("bited");
    max_quads(quad_count);
    push_constants(upc);
    refresh_atlas();
    refresh_batch();
  }
} i;
