#include <pebble.h>
#include "calculation/calculation.h"
#include "event/event.h"
#include "health/health.h"
#include "state/global.h"
#include "state/state.h"
#include "wakeup/wakeup.h"
#include "window/main.h"

static Window *s_window;

static void prv_init(void) {
  State* state = state_init();
  event_init(state);
  health_init(state);
  calculation_init(state);
  if (!wakeup_init(state)) {
    return;
  }
  main_init(state);

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = main_load,
    .unload = main_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);
}

static void prv_deinit(void) {
  window_destroy(s_window);

  state_write();
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
