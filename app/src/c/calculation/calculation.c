#include <pebble.h>
#include "calculation.h"

void calculation_init(State* state) {
  uint16_t step_target = state->settings->step_target;
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

  HealthValue steps = state->health->steps;

  state->calculation->step_target = step_target;
  state->calculation->step_warning = steps > step_target / 2 && steps + state->health->steps_todo < step_target;
  state->calculation->step_compliment = steps >= step_target;

  int active = state->health->active;
  int active_target = state->settings->active_target;

  if (state->settings->active_compensation) {
     active_target *= state->event->hours_done;
  } else {
     active -= state->health_persistent->active_last;
  }

  state->calculation->active = active / SECONDS_PER_MINUTE;
  state->calculation->active_target = active_target;
  state->calculation->active_warning = active < active_target * SECONDS_PER_MINUTE;
}
