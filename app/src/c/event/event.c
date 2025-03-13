#include <pebble.h>
#include "event.h"

void event_init(State* state) {
  state->event->now = time(NULL);

  tm* next = localtime(&state->event->now);

  int hours_done = 0;
  if (next->tm_hour > state->settings->begin) {
    if (next->tm_hour > state->settings->end && state->settings->end > state->settings->begin) {
      hours_done = state->settings->end - state->settings->begin;
    } else {
      hours_done = next->tm_hour - state->settings->begin;
    }
  }

  state->event->day = next->tm_mday;
  state->event->hours = next->tm_hour;
  state->event->hours_done = hours_done;
  state->event->muted_warnings = next->tm_hour < state->settings->begin || next->tm_hour > state->settings->end;

  next->tm_hour++;
  next->tm_min = state->settings->wakeup_deviation ? rand() % state->settings->wakeup_deviation : 0;
  next->tm_sec = 0;

  state->event->muted_next = next->tm_hour < state->settings->begin || next->tm_hour > state->settings->end;
  state->event->next = mktime(next);

  tm* sleep = localtime(&state->event->now);

  sleep->tm_hour = state->settings->sleep_end_hour;
  sleep->tm_min = state->settings->sleep_end_minute;
  state->event->sleep_end = mktime(sleep);
  state->event->sleep_day = next->tm_mday;
  if (state->event->sleep_end < state->event->now) {
    state->event->sleep_end += SECONDS_PER_DAY;
    state->event->sleep_day++;
  }
}
