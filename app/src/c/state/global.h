#pragma once
#include <pebble.h>

typedef struct {
  HealthValue steps;
  HealthValue steps_week;
  HealthValue steps_todo;
  HealthValue active;
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
} Settings;

typedef struct {
  time_t now;
  time_t next;
  uint16_t day;
  uint16_t hours;
  uint16_t hours_done;
  bool muted_warnings;
} Event;

typedef struct {
  uint16_t step_target;
  bool step_warning;
  bool step_compliment;
  uint16_t active;
  uint16_t active_target;
  bool active_warning;
} Calculation;

typedef struct {
  WakeupId wakeup;
  short muted_step_warning;
  short muted_step_compliment;
  Health* health;
  HealthPersistent* health_persistent;
  Settings* settings;
  Event* event;
  Calculation* calculation;
} State;
