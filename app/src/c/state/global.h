#pragma once
#include <pebble.h>

typedef struct {
  WakeupId wakeup;
  time_t now;
  time_t next;
} State;
