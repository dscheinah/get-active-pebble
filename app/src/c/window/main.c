#include <pebble.h>
#include "main.h"
#include "helper.h"

#define FONT FONT_KEY_GOTHIC_14_BOLD
#define FONT_SIZE 18
#define FONT_SIZE_HALF 9
#define BUFFER_SIZE 16
#define ICON_SIZE 24
#define ICON_SIZE_HALF 12
#define BAR_WIDTH 102
#define BAR_WIDTH_HALF 51
#define BAR_HEIGHT 9
#define GAP 18
#define GAP_SMALL 6
#define ICON_OFFSET (GAP + BAR_HEIGHT + FONT_SIZE + GAP_SMALL)
#define TEXT_OFFSET (GAP + BAR_HEIGHT)

static State* global;

static Layer* layer_steps;
static Layer* layer_sleep;
static Layer* layer_active;

static BitmapLayer* bitmap_layer_steps;
static BitmapLayer* bitmap_layer_sleep;
static BitmapLayer* bitmap_layer_active;

static GBitmap* bitmap_steps;
static GBitmap* bitmap_sleep;
static GBitmap* bitmap_active;

static TextLayer* text_layer_steps;
static TextLayer* text_layer_sleep;
static TextLayer* text_layer_active;

static char buffer_steps[BUFFER_SIZE] = "";
static char buffer_sleep[BUFFER_SIZE] = "";
static char buffer_active[BUFFER_SIZE] = "";

static void rect(Layer* layer, GContext* ctx) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_rect(ctx, GRect(0, 0, bounds.size.w, bounds.size.h));
}

static void steps(Layer* layer, GContext* ctx) {
  rect(layer, ctx);

  if (global->calculation->step_forecast < global->calculation->step_target) {
    GRect bounds = layer_get_bounds(layer);

    int pos = (bounds.size.w * global->calculation->step_forecast / global->calculation->step_target);

    graphics_context_set_stroke_color(ctx, COLOR_FALLBACK(GColorLightGray, GColorDarkGray));
    graphics_draw_line(ctx, GPoint(pos, 1), GPoint(pos, bounds.size.h - 2));
  }

  helper_render_percentage_rect(layer, ctx, global->calculation->step_warning, global->health->steps * 100 / global->calculation->step_target);
}

static void active(Layer* layer, GContext* ctx) {
  rect(layer, ctx);
  helper_render_percentage_rect(layer, ctx, global->calculation->active_warning, global->calculation->active * 100 / global->calculation->active_target);
}

inline static bool use_sleep_duration() {
  return global->calculation->sleep_distance < SECONDS_PER_90_MINUTES;
}

void main_init(State* state) {
  global = state;

  tm* sleep = gmtime(use_sleep_duration() ? &state->calculation->sleep_duration : &state->calculation->sleep_distance);

  snprintf(buffer_steps, BUFFER_SIZE, "%d / %d", (int) state->health->steps, state->calculation->step_target);
  strftime(buffer_sleep, BUFFER_SIZE, "%H:%M", sleep);
  snprintf(buffer_active, BUFFER_SIZE, "%d / %d", state->calculation->active, state->calculation->active_target);
}

void main_load(Window* window) {
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GFont font = fonts_get_system_font(FONT);
  GSize sleep = graphics_text_layout_get_content_size(buffer_sleep, font, bounds, GTextOverflowModeFill, GTextAlignmentCenter);

  bitmap_steps = gbitmap_create_with_resource(global->calculation->step_compliment ? RESOURCE_ID_compliment : RESOURCE_ID_steps);
  bitmap_sleep = gbitmap_create_with_resource(use_sleep_duration() ? RESOURCE_ID_sleep : RESOURCE_ID_awake);
  bitmap_active = gbitmap_create_with_resource(RESOURCE_ID_active);

  int cw = bounds.size.w / 2;
  int ch = bounds.size.h / 2;

  int sleep_w = sleep.w + GAP_SMALL;
  int sleep_o_min = cw - BAR_WIDTH_HALF;
  int sleep_o_max = cw + BAR_WIDTH_HALF - sleep_w - FONT_SIZE;
  int sleep_o = use_sleep_duration() ? sleep_o_max : sleep_o_min + ((sleep_o_max - sleep_o_min) * global->event->hours_done / (HOURS_PER_DAY - global->settings->begin));

  layer_steps = helper_layer_create(window_layer, GRect(cw - BAR_WIDTH_HALF, ch - GAP - BAR_HEIGHT, BAR_WIDTH, BAR_HEIGHT), steps);
  layer_sleep = helper_layer_create(window_layer, GRect(0, ch, bounds.size.w, 2), rect);
  layer_active = helper_layer_create(window_layer, GRect(cw - BAR_WIDTH_HALF, ch + GAP, BAR_WIDTH, BAR_HEIGHT), active);

  bitmap_layer_steps = helper_bitmap_layer_create(window_layer, GRect(cw - ICON_SIZE_HALF, ch - ICON_OFFSET - ICON_SIZE, ICON_SIZE, ICON_SIZE), bitmap_steps);
  bitmap_layer_sleep = helper_bitmap_layer_create(window_layer, GRect(sleep_o, ch - FONT_SIZE_HALF, FONT_SIZE, FONT_SIZE), bitmap_sleep);
  bitmap_layer_active = helper_bitmap_layer_create(window_layer, GRect(cw - ICON_SIZE_HALF, ch + ICON_OFFSET, ICON_SIZE, ICON_SIZE), bitmap_active);

  text_layer_steps = helper_text_layer_create(window_layer, GRect(10, ch - TEXT_OFFSET - FONT_SIZE, bounds.size.w - 20, FONT_SIZE), font, buffer_steps);
  text_layer_sleep = helper_text_layer_create(window_layer, GRect(sleep_o + FONT_SIZE, ch - FONT_SIZE_HALF, sleep_w, FONT_SIZE), font, buffer_sleep);
  text_layer_active = helper_text_layer_create(window_layer, GRect(10, ch + TEXT_OFFSET, bounds.size.w - 20, FONT_SIZE), font, buffer_active);

  if (global->calculation->step_warning) {
    text_layer_set_text_color(text_layer_steps, COLOR_FALLBACK(GColorDarkCandyAppleRed, GColorDarkGray));
  }
  if (global->calculation->sleep_warning) {
    text_layer_set_text_color(text_layer_sleep, COLOR_FALLBACK(GColorDarkCandyAppleRed, GColorDarkGray));
  }
  if (global->calculation->active_warning) {
    text_layer_set_text_color(text_layer_active, COLOR_FALLBACK(GColorDarkCandyAppleRed, GColorDarkGray));
  }
}

void main_unload() {
  text_layer_destroy(text_layer_steps);
  text_layer_destroy(text_layer_sleep);
  text_layer_destroy(text_layer_active);

  layer_destroy(layer_steps);
  layer_destroy(layer_sleep);
  layer_destroy(layer_active);

  bitmap_layer_destroy(bitmap_layer_steps);
  bitmap_layer_destroy(bitmap_layer_sleep);
  bitmap_layer_destroy(bitmap_layer_active);

  gbitmap_destroy(bitmap_steps);
  gbitmap_destroy(bitmap_sleep);
  gbitmap_destroy(bitmap_active);
}
