#include <pebble.h>
#include "wakeup.h"

#define MAX_TRY 23
#define TIMEOUT 30000

static State* global;

static void timeout() {
  window_stack_pop_all(false);
}

static void app_glance(AppGlanceReloadSession* session, size_t limit, void* context) {
  if (limit < 1) {
    return;
  }
  const AppGlanceSlice slice = {
    .layout = {
      .icon = APP_GLANCE_SLICE_DEFAULT_ICON,
      .subtitle_template_string = "Running",
    },
    .expiration_time = global->next,
  };
  app_glance_add_slice(session, slice);
}

static void wakeup_handler() {
  if (global->wakeup >= 0 && wakeup_query(global->wakeup, NULL)) {
    return;
  }
  time_t timeout = global->next;
  for (int try = 0; try < MAX_TRY; try++) {
     global->wakeup = wakeup_schedule(timeout, 0, true);
     if (global->wakeup >= 0) {
       global->next = timeout;
       app_glance_reload(app_glance, NULL);
       break;
     }
     timeout += SECONDS_PER_MINUTE;
  }
}

void wakeup_init(State* state) {
  global = state;

  wakeup_service_subscribe(wakeup_handler);
  wakeup_handler();

  if (launch_reason() != APP_LAUNCH_WAKEUP) {
    return;
  }
  exit_reason_set(APP_EXIT_ACTION_PERFORMED_SUCCESSFULLY);
  app_timer_register(TIMEOUT, timeout, NULL);

  if (quiet_time_is_active()) {
    return;
  }
  vibes_long_pulse();
}
