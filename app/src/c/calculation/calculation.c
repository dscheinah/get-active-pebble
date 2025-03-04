#include <pebble.h>
#include "calculation.h"

inline int seconds(int minutes) {
  return minutes * SECONDS_PER_MINUTE;
}

inline int minutes(int seconds) {
  return seconds / SECONDS_PER_MINUTE;
}

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

  int steps_per_hour = state->event->hours ? state->health->steps / state->event->hours : 0;

  state->calculation->step_target = step_target;
  state->calculation->step_warning = state->health->steps + (steps_per_hour * state->event->hours_left) < step_target;
  state->calculation->step_compliment = state->health->steps >= step_target;

  int active_current = state->health->active - state->health_persistent->active_last;
  int active = active_current;
  if (state->settings->active_compensation) {
    int remaining = state->health_persistent->active_last - (state->event->hours_done * seconds(state->settings->active_target));
    if (remaining > 0) {
      active += remaining;
    }
  }

  state->calculation->active = minutes(active);
  state->calculation->active_current = minutes(active_current);
  state->calculation->active_warning = active < state->settings->active_target;
}
