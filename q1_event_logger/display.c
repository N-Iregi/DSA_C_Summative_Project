#include "event_logger.h"

/**
 * type_name - text for an event type
 * @t: event type
 *
 * Return: constant string naming the type
 */
const char *type_name(event_type_t t)
{
	switch (t)
	{
	case EV_TEMP_WARNING:
		return ("TEMPERATURE_WARNING");
	case EV_LOW_BATTERY:
		return ("LOW_BATTERY");
	case EV_SENSOR_FAILURE:
		return ("SENSOR_FAILURE");
	case EV_COMM_ERROR:
		return ("COMMUNICATION_ERROR");
	}
	return ("UNKNOWN");
}

/**
 * severity_name - text for a severity level
 * @s: severity
 *
 * Return: constant string naming the severity
 */
const char *severity_name(severity_t s)
{
	switch (s)
	{
	case SEV_LOW:
		return ("LOW");
	case SEV_MEDIUM:
		return ("MEDIUM");
	case SEV_HIGH:
		return ("HIGH");
	case SEV_CRITICAL:
		return ("CRITICAL");
	}
	return ("UNKNOWN");
}

/**
 * format_time - write a timestamp as YYYY-MM-DD HH:MM:SS
 * @t: time to format
 * @out: destination buffer
 * @size: size of @out
 */
void format_time(time_t t, char *out, size_t size)
{
	struct tm *info;

	info = localtime(&t);
	if (info == NULL ||
			strftime(out, size, "%Y-%m-%d %H:%M:%S", info) == 0)
		snprintf(out, size, "unknown");
}

/**
 * print_event - print every field of one event
 * @lg: logger the event belongs to
 * @e: event to print
 */
void print_event(const logger_t *lg, const event_t *e)
{
	char when[32];

	format_time(e->timestamp, when, sizeof(when));
	printf("--- Event %d of %d%s%s ---\n", logger_position(lg, e),
			lg->count, e == lg->head ? " (oldest)" : "",
			e == lg->tail ? " (newest)" : "");
	printf("  Event ID  : %u\n", e->id);
	printf("  Type      : %s\n", type_name(e->type));
	printf("  Device    : %s\n", e->device);
	printf("  Timestamp : %s\n", when);
	printf("  Severity  : %s\n", severity_name(e->severity));
}

/**
 * cmd_list - list all events, oldest first
 * @lg: logger to list
 */
void cmd_list(const logger_t *lg)
{
	const event_t *e;
	char when[32];

	if (lg->head == NULL)
	{
		puts("The log is empty.");
		return;
	}
	printf("%d event(s), oldest first ('>' = current):\n", lg->count);
	for (e = lg->head; e != NULL; e = e->next)
	{
		format_time(e->timestamp, when, sizeof(when));
		printf(" %c #%-4u %s  %-20s %-12s %s\n",
				e == lg->current ? '>' : ' ', e->id, when,
				type_name(e->type), e->device,
				severity_name(e->severity));
	}
}
