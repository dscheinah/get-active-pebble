#include <pebble.h>
#include "main.h"

#define HEIGHT 25
#define FONT FONT_KEY_GOTHIC_18_BOLD

static TextLayer* text_layer;

void main_load(Window* window) {
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create(GRect(10, (bounds.size.h - HEIGHT) / 2, bounds.size.w - 20, HEIGHT));
  text_layer_set_text(text_layer, "Wakeup");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT));

  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

void main_unload() {
  text_layer_destroy(text_layer);
}
