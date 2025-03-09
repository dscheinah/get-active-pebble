#include <pebble.h>
#include "event.h"

void event_init(State* state) {
  state->event->now = time(NULL);

  tm* now = localtime(&state->event->now);

  state->event->day = now->tm_mday;
  state->event->hours_done = now->tm_hour > state->settings->begin ? now->tm_hour - state->settings->begin : 0;
  state->event->muted_warnings = now->tm_hour < state->settings->begin || now->tm_hour > state->settings->end;

  now->tm_hour++;
  now->tm_min = state->settings->wakeup_deviation ? rand() % state->settings->wakeup_deviation : 0;
  now->tm_sec = 0;

  state->event->next = mktime(now);
  state->event->muted_next = now->tm_hour < state->settings->begin || now->tm_hour > state->settings->end;

  now->tm_hour = state->settings->sleep_end_hour;
  now->tm_min = state->settings->sleep_end_minute;
  state->event->sleep_end = mktime(now);
  if (state->event->sleep_end < state->event->now) {
    now->tm_mday++;
    state->event->sleep_end = mktime(now);
  }
}
