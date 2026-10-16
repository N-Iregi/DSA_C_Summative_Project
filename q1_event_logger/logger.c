#include "event_logger.h"

/**
 * logger_init - put the logger into its empty state
 * @lg: logger to initialise
 */
void logger_init(logger_t *lg) {
  lg->head = NULL;
  lg->tail = NULL;
  lg->current = NULL;
  lg->count = 0;
  lg->next_id = 1;
}

/**
 * event_create - allocate and fill one event node
 * @id: event number
 * @type: kind of event
 * @device: sensor or device identifier
 * @severity: how serious the event is
 * @timestamp: time of the event
 *
 * Return: the new node, or NULL if malloc fails
 */
event_t *event_create(unsigned int id, event_type_t type, const char *device,
                      severity_t severity, time_t timestamp) {
  event_t *ev;

  ev = malloc(sizeof(event_t));
  if (ev == NULL)
    return (NULL);
  ev->id = id;
  ev->type = type;
  snprintf(ev->device, sizeof(ev->device), "%s", device);
  ev->timestamp = timestamp;
  ev->severity = severity;
  ev->prev = NULL;
  ev->next = NULL;
  return (ev);
}

/**
 * logger_evict_oldest - remove and free the oldest event
 * @lg: logger to shrink
 *
 * Repairs head, tail and current so none of them dangles.
 */
void logger_evict_oldest(logger_t *lg) {
  event_t *oldest = lg->head;

  if (oldest == NULL)
    return;
  printf("Log full (%d events): oldest event #%u removed.\n", MAX_EVENTS,
         oldest->id);
  if (lg->current == oldest) {
    lg->current = oldest->next;
    if (lg->current != NULL)
      printf("Current moved to #%u.\n", lg->current->id);
  }
  lg->head = oldest->next;
  if (lg->head != NULL)
    lg->head->prev = NULL;
  else
    lg->tail = NULL;
  free(oldest);
  lg->count--;
}

/**
 * logger_add - append a new event at the tail of the log
 * @lg: logger to add to
 * @type: kind of event
 * @device: sensor or device identifier
 * @severity: how serious the event is
 * @timestamp: time of the event
 *
 * Return: the new event, or NULL if memory ran out
 */
event_t *logger_add(logger_t *lg, event_type_t type, const char *device,
                    severity_t severity, time_t timestamp) {
  event_t *ev;

  ev = event_create(lg->next_id, type, device, severity, timestamp);
  if (ev == NULL)
    return (NULL);
  lg->next_id++;
  if (lg->count >= MAX_EVENTS)
    logger_evict_oldest(lg);
  ev->prev = lg->tail;
  if (lg->tail != NULL)
    lg->tail->next = ev;
  else
    lg->head = ev;
  lg->tail = ev;
  lg->count++;
  if (lg->current == NULL)
    lg->current = ev;
  return (ev);
}

/**
 * logger_clear - free every node and reset the logger
 * @lg: logger to empty
 */
void logger_clear(logger_t *lg) {
  event_t *e, *next;

  e = lg->head;
  while (e != NULL) {
    next = e->next;
    free(e);
    e = next;
  }
  lg->head = NULL;
  lg->tail = NULL;
  lg->current = NULL;
  lg->count = 0;
}
