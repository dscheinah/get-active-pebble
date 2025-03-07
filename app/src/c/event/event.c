#include <pebble.h>
#include "event.h"

void event_init(State* state) {
  state->event->now = time(NULL);

  tm* now = localtime(&state->event->now);

  int hour = now->tm_hour;
  int begin = state->settings->begin;
  int end = state->settings->end;

  state->event->day = now->tm_mday;
  state->event->hours = hour;
  state->event->hours_done = hour > begin ? hour - begin : 0;
  state->event->muted_warnings = hour < begin || hour > end;

  now->tm_hour++;
  now->tm_min = 0;
  now->tm_sec = 0;

  if (state->settings->wakeup_deviation) {
    now->tm_min = rand() % state->settings->wakeup_deviation;
  }

  state->event->next = mktime(now);

  now->tm_hour = state->settings->sleep_end_hour;
  now->tm_min = state->settings->sleep_end_minute;
  state->event->sleep_end = mktime(now);
  if (state->event->sleep_end < state->event->now) {
    now->tm_mday++;
    state->event->sleep_end = mktime(now);
  }
}
