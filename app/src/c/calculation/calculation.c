#include <pebble.h>
#include "calculation.h"

void calculation_init(State* state) {
  int step_target = state->settings->step_target;
  if (state->settings->step_deviation) {
    int offset = state->settings->step_target * state->settings->step_deviation / 100;

    step_target = state->health->steps_week;

    int lower = state->settings->step_target - offset;
    if (step_target < lower) {
      step_target = lower;
    }
    int upper = state->settings->step_target + offset;
    if (step_target > upper) {
      step_target = upper;
    }
  }

  state->calculation->step_target = step_target;
  state->calculation->step_forecast = state->health->steps + state->health->steps_todo;
  state->calculation->step_warning = state->health->steps > step_target / 3 && state->calculation->step_forecast < step_target * 100 / 110;
  state->calculation->step_compliment = state->health->steps >= step_target;

  int active = state->health->active;
  int active_target = state->settings->active_target;

  if (state->settings->active_compensation) {
     active_target *= state->event->hours_done;
  } else {
     active -= state->health_persistent->active_last;
  }

  state->calculation->active = active / SECONDS_PER_MINUTE;
  state->calculation->active_target = active_target;
  state->calculation->active_warning = state->calculation->active < state->calculation->active_target;

  int sleep_target = state->settings->sleep_target * SECONDS_PER_90_MINUTES;
  if (state->settings->sleep_compensation && state->health->sleep_week && state->health->sleep_week < sleep_target - SECONDS_PER_30_MINUTES) {
    sleep_target += SECONDS_PER_90_MINUTES;
  }
  state->calculation->sleep_duration = state->event->sleep_end - state->event->now;
  state->calculation->sleep_distance = state->calculation->sleep_duration > sleep_target ? state->calculation->sleep_duration - sleep_target : 0;
  state->calculation->sleep_warning = state->event->sleep_end - state->event->next < sleep_target;
}
