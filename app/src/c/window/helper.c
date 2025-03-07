#include <pebble.h>
#include "helper.h"

TextLayer* helper_text_layer_create(Layer* parent, GRect rect, GFont font, char* text) {
  TextLayer* layer = text_layer_create(rect);
  text_layer_set_text(layer, text);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer, GTextAlignmentCenter);
  layer_add_child(parent, text_layer_get_layer(layer));
  return layer;
}

BitmapLayer* helper_bitmap_layer_create(Layer* parent, GRect rect, GBitmap* bitmap) {
  BitmapLayer* layer = bitmap_layer_create(rect);
  bitmap_layer_set_bitmap(layer, bitmap);
  layer_add_child(parent, bitmap_layer_get_layer(layer));
  return layer;
}

Layer* helper_layer_create(Layer* parent, GRect rect, LayerUpdateProc update_proc) {
  Layer* layer = layer_create(rect);
  layer_set_update_proc(layer, update_proc);
  layer_add_child(parent, layer);
  return layer;
}

void helper_render_percentage_rect(Layer* layer, GContext* ctx, bool warning, int percentage) {
  if (percentage > 100) {
    percentage = 100;
  }
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, warning ? COLOR_FALLBACK(GColorDarkCandyAppleRed, GColorBlack) : COLOR_FALLBACK(GColorBlack, GColorLightGray));
  graphics_fill_rect(ctx, GRect(1, 1, (bounds.size.w * percentage / 100) - 2, bounds.size.h - 2), 0, GCornerNone);
}
