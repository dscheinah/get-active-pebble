#include <pebble.h>
#include "wakeup.h"

#define MAX_TRY 23
#define TIMEOUT 30000
#define BUFFER_SIZE 64

static State* global;

static char buffer[BUFFER_SIZE] = "";

inline static bool notify(State* state) {
  if (state->calculation->sleep_warning && state->settings->sleep_warning) {
    if (state->muted_sleep_warning == state->event->day) {
      return false;
    }
    state->muted_sleep_warning = state->event->day;
    return true;
  }
  if (state->calculation->step_warning && state->settings->step_warning) {
    if (state->muted_step_warning == state->event->day) {
      return false;
    }
    state->muted_step_warning = state->event->day;
    return true;
  }
  if (state->calculation->step_compliment && state->settings->step_compliment) {
    if (state->muted_step_compliment == state->event->day) {
      return false;
    }
    state->muted_step_compliment = state->event->day;
    return true;
  }
  if (state->calculation->active_warning && state->settings->active_warning) {
    if (state->event->muted_warnings) {
      return false;
    }
    return true;
  }
  return false;
}

static void timeout() {
  window_stack_pop_all(false);
}

static void app_glance(AppGlanceReloadSession* session, size_t limit, void* context) {
  if (limit < 1) {
    return;
  }

  snprintf(buffer, BUFFER_SIZE, "{time_until(%d)|format('%%aR')} - %s", (int) global->event->next, global->event->muted_next ? "silent" : "active");

  const AppGlanceSlice slice = {
    .layout = {
      .icon = APP_GLANCE_SLICE_DEFAULT_ICON,
      .subtitle_template_string = buffer,
    },
    .expiration_time = global->event->next,
  };
  app_glance_add_slice(session, slice);
}

static void wakeup_handler() {
  if (global->wakeup >= 0 && wakeup_query(global->wakeup, NULL)) {
    return;
  }
  time_t timeout = global->event->next;
  for (int try = 0; try < MAX_TRY; try++) {
     global->wakeup = wakeup_schedule(timeout, 0, true);
     if (global->wakeup >= 0) {
       global->event->next = timeout;
       app_glance_reload(app_glance, NULL);
       break;
     }
     timeout += SECONDS_PER_MINUTE;
  }
}

bool wakeup_init(State* state) {
  global = state;

  wakeup_service_subscribe(wakeup_handler);
  wakeup_handler();

  if (launch_reason() != APP_LAUNCH_WAKEUP) {
    return true;
  }
  exit_reason_set(APP_EXIT_ACTION_PERFORMED_SUCCESSFULLY);
  if (!notify(state)) {
    return false;
  }
  app_timer_register(TIMEOUT, timeout, NULL);

  if (!quiet_time_is_active()) {
    vibes_long_pulse();
  }
  return true;
}
