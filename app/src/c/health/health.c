#include <pebble.h>
#include "health.h"

void health_init(State* state) {
  time_t now = state->event->now;
  time_t start = time_start_of_today();
  time_t end = start + SECONDS_PER_DAY;

  state->health->steps = health_service_sum_today(HealthMetricStepCount);
  state->health->steps_week = health_service_sum_averaged(HealthMetricStepCount, start, end, HealthServiceTimeScopeDaily);
  state->health->steps_todo = health_service_sum_averaged(HealthMetricStepCount, now, end, HealthServiceTimeScopeDaily);

  state->health->active = health_service_sum_today(HealthMetricActiveSeconds);
  if (state->health_persistent->wakeup != state->wakeup) {
    state->health_persistent->wakeup = state->wakeup;
    state->health_persistent->active_last = state->health_persistent->active_current;
    state->health_persistent->active_current = state->health->active;
  }
}
