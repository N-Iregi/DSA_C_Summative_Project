#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_EVENTS 25
#define DEVICE_LEN 32
#define LINE_LEN 128
#define SESSION_FILE "event_session.log"

/**
 * enum event_type_e - kinds of event a device can report
 * @EV_TEMP_WARNING: temperature warning
 * @EV_LOW_BATTERY: low-battery alert
 * @EV_SENSOR_FAILURE: sensor failure
 * @EV_COMM_ERROR: communication error
 */
typedef enum event_type_e
{
	EV_TEMP_WARNING = 1,
	EV_LOW_BATTERY,
	EV_SENSOR_FAILURE,
	EV_COMM_ERROR
} event_type_t;

/**
 * enum severity_e - how serious an event is
 * @SEV_LOW: low severity
 * @SEV_MEDIUM: medium severity
 * @SEV_HIGH: high severity
 * @SEV_CRITICAL: critical severity
 */
typedef enum severity_e
{
	SEV_LOW = 1,
	SEV_MEDIUM,
	SEV_HIGH,
	SEV_CRITICAL
} severity_t;

/**
 * struct event_s - one logged event (a doubly linked list node)
 * @id: unique, ever-increasing event number
 * @type: kind of event
 * @device: sensor or device identifier
 * @timestamp: when the event was recorded
 * @severity: how serious the event is
 * @prev: next older event (NULL for the oldest)
 * @next: next newer event (NULL for the newest)
 */
typedef struct event_s
{
	unsigned int id;
	event_type_t type;
	char device[DEVICE_LEN];
	time_t timestamp;
	severity_t severity;
	struct event_s *prev;
	struct event_s *next;
} event_t;

/**
 * struct logger_s - the event log and its three pointers
 * @head: oldest event
 * @tail: newest event
 * @current: event the operator is looking at
 * @count: number of events stored (never above MAX_EVENTS)
 * @next_id: id given to the next event created
 */
typedef struct logger_s
{
	event_t *head;
	event_t *tail;
	event_t *current;
	int count;
	unsigned int next_id;
} logger_t;

/* input.c */
int read_line(char *buf, size_t size);
char *trim(char *s);

/* logger.c */
void logger_init(logger_t *lg);
event_t *event_create(unsigned int id, event_type_t type,
		const char *device, severity_t severity, time_t timestamp);
void logger_evict_oldest(logger_t *lg);
event_t *logger_add(logger_t *lg, event_type_t type, const char *device,
		severity_t severity, time_t timestamp);
void logger_clear(logger_t *lg);

/* navigate.c */
int logger_position(const logger_t *lg, const event_t *target);
void cmd_next(logger_t *lg);
void cmd_prev(logger_t *lg);
void cmd_show(const logger_t *lg);
void cmd_clear(logger_t *lg);

/* display.c */
const char *type_name(event_type_t t);
const char *severity_name(severity_t s);
void format_time(time_t t, char *out, size_t size);
void print_event(const logger_t *lg, const event_t *e);
void cmd_list(const logger_t *lg);

/* add_event.c */
int prompt_choice(const char *prompt, int lo, int hi);
int valid_device_id(const char *s);
int prompt_device(char *device);
void cmd_add(logger_t *lg);

/* session.c */
int logger_save(const logger_t *lg);
void end_session(logger_t *lg);

/* main.c */
void print_help(void);
void run_command(logger_t *lg, char cmd);

#endif /* EVENT_LOGGER_H */
