#include "event_logger.h"

/**
 * logger_position - find the 1-based position of an event
 * @lg: logger to search
 * @target: event to locate
 *
 * Return: position counted from the oldest event, 0 if not found
 */
int logger_position(const logger_t *lg, const event_t *target) {
  const event_t *e;
  int pos = 1;

  for (e = lg->head; e != NULL; e = e->next, pos++) {
    if (e == target)
      return (pos);
  }
  return (0);
}

/**
 * cmd_next - move current to the next (newer) event
 * @lg: logger to browse
 */
void cmd_next(logger_t *lg) {
  if (lg->current == NULL)
    puts("The log is empty - no events to browse.");
  else if (lg->current->next == NULL)
    puts("Already at the newest event - cannot move further.");
  else {
    lg->current = lg->current->next;
    print_event(lg, lg->current);
  }
}

/**
 * cmd_prev - move current to the previous (older) event
 * @lg: logger to browse
 */
void cmd_prev(logger_t *lg) {
  if (lg->current == NULL)
    puts("The log is empty - no events to browse.");
  else if (lg->current->prev == NULL)
    puts("Already at the oldest event - cannot move further.");
  else {
    lg->current = lg->current->prev;
    print_event(lg, lg->current);
  }
}

/**
 * cmd_show - display the current event
 * @lg: logger to read
 */
void cmd_show(const logger_t *lg) {
  if (lg->current == NULL)
    puts("The log is empty - no current event.");
  else
    print_event(lg, lg->current);
}

/**
 * cmd_clear - delete all events
 * @lg: logger to empty
 */
void cmd_clear(logger_t *lg) {
  int n;

  if (lg->count == 0) {
    puts("The log is already empty.");
    return;
  }
  n = lg->count;
  logger_clear(lg);
  printf("Cleared %d event(s); all memory released.\n", n);
}
