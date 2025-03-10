#include <pebble.h>
#include "health.h"

void health_init(State* state) {
  time_t now = state->event->now;
  time_t start = time_start_of_today();
  time_t end = start + SECONDS_PER_DAY;

  state->health->steps = health_service_sum_today(HealthMetricStepCount);

  HealthServiceAccessibilityMask mask;

  int steps_per_hour = 0;

  mask = health_service_metric_averaged_accessible(HealthMetricStepCount, start, end, HealthServiceTimeScopeDaily);
  if (mask & HealthServiceAccessibilityMaskAvailable) {
    state->health->steps_week = health_service_sum_averaged(HealthMetricStepCount, start, end, HealthServiceTimeScopeDaily);
    steps_per_hour = state->health->steps_week / HOURS_PER_DAY;
  } else {
    state->health->steps_week = state->settings->step_target;
  }
  if (state->event->hours) {
    if (!steps_per_hour) {
      steps_per_hour = state->health->steps / state->event->hours;
    } else {
      steps_per_hour = (steps_per_hour + (state->health->steps / state->event->hours)) / 2;
    }
  }
  state->health->steps_todo = steps_per_hour * (HOURS_PER_DAY - state->event->hours_done);

  state->health->active = health_service_sum_today(HealthMetricActiveSeconds);
  if (state->health_persistent->wakeup != state->wakeup) {
    state->health_persistent->wakeup = state->wakeup;
    state->health_persistent->active_last = state->health_persistent->active_current;
    state->health_persistent->active_current = state->health->active;
  }

  mask = health_service_metric_averaged_accessible(HealthMetricSleepSeconds, start, end, HealthServiceTimeScopeDaily);
  if (mask & HealthServiceAccessibilityMaskAvailable) {
    state->health->sleep_week = health_service_sum_averaged(HealthMetricSleepSeconds, start, end, HealthServiceTimeScopeDaily);
  } else {
    state->health->sleep_week = 0;
  }
}
