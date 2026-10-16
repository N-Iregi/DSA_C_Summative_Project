#include "event_logger.h"

/**
 * logger_save - write the whole log to SESSION_FILE, oldest first
 * @lg: logger to save
 *
 * Return: 1 on success, 0 on failure
 */
int logger_save(const logger_t *lg)
{
	FILE *fp;
	const event_t *e;
	char now[32], when[32];

	fp = fopen(SESSION_FILE, "w");
	if (fp == NULL)
		return (0);
	format_time(time(NULL), now, sizeof(now));
	fprintf(fp, "# Session saved %s - %d event(s)\n", now, lg->count);
	fprintf(fp, "# id | timestamp | type | device | severity\n");
	for (e = lg->head; e != NULL; e = e->next)
	{
		format_time(e->timestamp, when, sizeof(when));
		fprintf(fp, "%u | %s | %s | %s | %s\n", e->id, when,
				type_name(e->type), e->device,
				severity_name(e->severity));
	}
	return (fclose(fp) == 0);
}

/**
 * end_session - save the log, free every node and say goodbye
 * @lg: logger to close
 */
void end_session(logger_t *lg)
{
	if (logger_save(lg))
		printf("Session saved to %s (%d event(s)).\n",
				SESSION_FILE, lg->count);
	else
		puts("Warning: could not write the session file.");
	logger_clear(lg);
	puts("Goodbye.");
}
