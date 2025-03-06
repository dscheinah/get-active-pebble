#include <pebble.h>
#include "settings.h"

State* global;

static void handle_inbox(DictionaryIterator* iter, void* context) {
  Tuple* step_target = dict_find(iter, MESSAGE_KEY_step_target);
  if (step_target) {
    global->settings->step_target = step_target->value->uint16;
  }
  Tuple* step_deviation = dict_find(iter, MESSAGE_KEY_step_deviation);
  if (step_deviation) {
    global->settings->step_deviation = step_deviation->value->uint16;
  }
  Tuple* step_warning = dict_find(iter, MESSAGE_KEY_step_warning);
  if (step_warning) {
    global->settings->step_warning = (bool) step_warning->value->int8;
  }
  Tuple* step_compliment = dict_find(iter, MESSAGE_KEY_step_compliment);
  if (step_compliment) {
    global->settings->step_compliment = (bool) step_compliment->value->int8;
  }
  Tuple* active_target = dict_find(iter, MESSAGE_KEY_active_target);
  if (active_target) {
    global->settings->active_target = active_target->value->uint16;
  }
  Tuple* active_compensation = dict_find(iter, MESSAGE_KEY_active_compensation);
  if (active_compensation) {
    global->settings->active_compensation = (bool) active_compensation->value->int8;
  }
  Tuple* active_warning = dict_find(iter, MESSAGE_KEY_active_warning);
  if (active_warning) {
    global->settings->active_warning = (bool) active_warning->value->int8;
  }
  Tuple* wakeup_deviation = dict_find(iter, MESSAGE_KEY_wakeup_deviation);
  if (wakeup_deviation) {
    global->settings->wakeup_deviation = wakeup_deviation->value->uint16;
  }
  Tuple* begin = dict_find(iter, MESSAGE_KEY_begin);
  if (begin) {
    global->settings->begin = begin->value->int16;
  }
  Tuple* end = dict_find(iter, MESSAGE_KEY_end);
  if (end) {
    global->settings->end = end->value->int16;
  }
  Tuple* sleep_target = dict_find(iter, MESSAGE_KEY_sleep_target);
  if (sleep_target) {
    global->settings->sleep_target = sleep_target->value->int8 - 48;
  }
  Tuple* sleep_end_hour = dict_find(iter, MESSAGE_KEY_sleep_end_hour);
  if (sleep_end_hour) {
    global->settings->sleep_end_hour = sleep_end_hour->value->int16;
  }
  Tuple* sleep_end_minute = dict_find(iter, MESSAGE_KEY_sleep_end_minute);
  if (sleep_end_minute) {
    global->settings->sleep_end_minute = sleep_end_minute->value->int16;
  }
  Tuple* sleep_compensation = dict_find(iter, MESSAGE_KEY_sleep_compensation);
  if (sleep_compensation) {
    global->settings->sleep_compensation = (bool) sleep_compensation->value->int8;
  }
  Tuple* sleep_warning = dict_find(iter, MESSAGE_KEY_sleep_warning);
  if (sleep_warning) {
    global->settings->sleep_warning = (bool) sleep_warning->value->int8;
  }

  window_stack_pop_all(false);
}

void settings_init(State* state) {
  global = state;

  app_message_register_inbox_received(handle_inbox);
  app_message_open(256, 128);
}

void settings_deinit() {
  app_message_deregister_callbacks();
}
