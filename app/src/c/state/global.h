#pragma once
#include <pebble.h>

#define SECONDS_PER_90_MINUTES 5400

typedef struct {
  HealthValue steps;
  HealthValue steps_week;
  HealthValue steps_todo;
  HealthValue active;
  HealthValue sleep_week;
} Health;

typedef struct {
  WakeupId wakeup;
  HealthValue active_last;
  HealthValue active_current;
} HealthPersistent;

typedef struct {
  uint16_t step_target;
  uint16_t step_deviation;
  bool step_warning;
  bool step_compliment;
  uint16_t active_target;
  bool active_compensation;
  bool active_warning;
  uint16_t wakeup_deviation;
  short begin;
  short end;
  short sleep_target;
  short sleep_end_hour;
  short sleep_end_minute;
  bool sleep_compensation;
  bool sleep_warning;
} Settings;

typedef struct {
  time_t now;
  time_t next;
  time_t sleep_end;
  uint16_t day;
  uint16_t hours_done;
  bool muted_warnings;
  bool muted_next;
} Event;

typedef struct {
  uint16_t step_target;
  bool step_warning;
  bool step_compliment;
  uint16_t active;
  uint16_t active_target;
  bool active_warning;
  time_t sleep_duration;
  time_t sleep_distance;
  bool sleep_warning;
} Calculation;

typedef struct {
  WakeupId wakeup;
  short muted_step_warning;
  short muted_step_compliment;
  short muted_sleep_warning;
  Health* health;
  HealthPersistent* health_persistent;
  Settings* settings;
  Event* event;
  Calculation* calculation;
} State;
