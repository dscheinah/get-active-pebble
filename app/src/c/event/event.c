#include "event.h"

void event_init(State* state) {
  state->now = time(NULL);

  tm* next = localtime(&state->now);
  next->tm_hour++;
  next->tm_min = 0;
  next->tm_sec = 0;

  state->next = mktime(next);
}
