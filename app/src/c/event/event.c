#include <pebble.h>
#include "event.h"

void event_init(State* state) {
  state->event->now = time(NULL);

  tm* next = localtime(&state->event->now);

  state->event->day = next->tm_mday;
  state->event->hours_done = next->tm_hour > state->settings->begin ? next->tm_hour - state->settings->begin : 0;
  state->event->muted_warnings = next->tm_hour < state->settings->begin || next->tm_hour > state->settings->end;

  next->tm_hour++;
  next->tm_min = state->settings->wakeup_deviation ? rand() % state->settings->wakeup_deviation : 0;
  next->tm_sec = 0;

  state->event->next = mktime(next);
  state->event->muted_next = next->tm_hour < state->settings->begin || next->tm_hour > state->settings->end;

  tm* sleep = localtime(&state->event->now);

  sleep->tm_hour = state->settings->sleep_end_hour;
  sleep->tm_min = state->settings->sleep_end_minute;
  state->event->sleep_end = mktime(sleep);
  if (state->event->sleep_end < state->event->now) {
    state->event->sleep_end += SECONDS_PER_DAY;
  }
}
