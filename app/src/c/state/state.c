#include <pebble.h>
#include "state.h"

#define VERSION_KEY  0
#define STATE_KEY    1
#define HEALTH_KEY   2
#define SETTINGS_KEY 3

static Health health = {
  .steps = 0,
  .steps_week = 0,
  .steps_todo = 0,
  .active = 0,
  .sleep_week = 0,
};

static HealthPersistent health_persistent = {
  .wakeup = -1,
  .active_last = 0,
  .active_current = 0,
};

static Settings settings = {
  .step_target = 15000,
  .step_deviation = 50,
  .step_warning = true,
  .step_compliment = true,
  .active_target = 6,
  .active_compensation = true,
  .active_warning = true,
  .wakeup_deviation = 20,
  .begin = 10,
  .end = 18,
  .sleep_target = 5,
  .sleep_end_hour = 7,
  .sleep_end_minute = 0,
  .sleep_compensation = true,
  .sleep_warning = true,
};

static Event event = {
  .now = 0,
  .next = 0,
  .sleep_end = 0,
  .day = 0,
  .hours = 0,
  .hours_done = 0,
  .muted_warnings = false,
};

static Calculation calculation = {
  .step_target = 0,
  .step_warning = false,
  .step_compliment = false,
  .active = 0,
  .active_target = 0,
  .active_warning = false,
  .sleep_duration = 0,
  .sleep_distance = 0,
  .sleep_warning = false,
};

static State state = {
  .wakeup = -2,
  .muted_step_warning = -1,
  .muted_step_compliment = -1,
  .muted_sleep_warning = -1,
};

State* state_init() {
  if (persist_exists(VERSION_KEY)) {
    persist_read_data(STATE_KEY, &state, sizeof(State));
    persist_read_data(HEALTH_KEY, &health_persistent, sizeof(HealthPersistent));
    persist_read_data(SETTINGS_KEY, &settings, sizeof(Settings));
  }
  state.health = &health;
  state.health_persistent = &health_persistent;
  state.settings = &settings;
  state.event = &event;
  state.calculation = &calculation;
  return &state;
}

void state_write() {
  persist_write_int(VERSION_KEY, 1);
  persist_write_data(STATE_KEY, &state, sizeof(State));
  persist_write_data(HEALTH_KEY, &health_persistent, sizeof(HealthPersistent));
  persist_write_data(SETTINGS_KEY, &settings, sizeof(Settings));
}
