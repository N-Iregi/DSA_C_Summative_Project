#include "registry.h"

/**
 * timestamp_now - write the current time as YYYY-MM-DD HH:MM:SS
 * @out: destination buffer
 * @size: size of @out
 */
void timestamp_now(char *out, size_t size)
{
	time_t now;
	struct tm *info;

	now = time(NULL);
	info = localtime(&now);
	if (info == NULL ||
			strftime(out, size, "%Y-%m-%d %H:%M:%S", info) == 0)
		snprintf(out, size, "0000-00-00 00:00:00");
}

/**
 * audit_log_unknown - append a rejected command to audit.log
 * @cmd: the command the technician typed
 */
void audit_log_unknown(const char *cmd)
{
	FILE *fp;
	char when[32];

	fp = fopen(AUDIT_LOG, "a");
	if (fp == NULL)
	{
		printf("Warning: could not write %s (%s).\n",
				AUDIT_LOG, strerror(errno));
		return;
	}
	timestamp_now(when, sizeof(when));
	fprintf(fp, "%s | UNKNOWN | %s\n", when, cmd);
	fclose(fp);
}

/**
 * save_session - write a short summary of the session
 * @reg: registry (for the command count)
 * @stats: verification results
 */
void save_session(const registry_t *reg, const stats_t *stats)
{
	FILE *fp;
	char when[32];

	fp = fopen(SUMMARY_FILE, "w");
	if (fp == NULL)
	{
		printf("Warning: could not write %s.\n", SUMMARY_FILE);
		return;
	}
	timestamp_now(when, sizeof(when));
	fprintf(fp, "Session ended: %s\n", when);
	fprintf(fp, "Commands in registry at exit: %d\n", reg->count);
	fprintf(fp, "Approved: %d\n", stats->approved);
	fprintf(fp, "Suggested (not approved): %d\n", stats->suggested);
	fprintf(fp, "Unknown: %d\n", stats->unknown);
	fprintf(fp, "Rejected attempts are recorded in %s\n", AUDIT_LOG);
	fclose(fp);
	printf("Session summary saved to %s.\n", SUMMARY_FILE);
}
