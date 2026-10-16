#include "event_logger.h"

/**
 * prompt_choice - ask for a number in a range
 * @prompt: text shown to the operator
 * @low: lowest accepted value
 * @high: highest accepted value
 *
 * Return: the number chosen, or 0 if the operator cancelled
 */
int prompt_choice(const char *prompt, int low, int high)
{
	char line[LINE_LEN], *s, *end;
	int r;
	long v;

	while (1)
	{
		printf("%s", prompt);
		r = read_line(line, sizeof(line));
		s = trim(line);
		if (r < 0 || *s == '\0')
			return (0);
		v = strtol(s, &end, 10);
		if (r == 0 && *end == '\0' && v >= low && v <= high)
			return ((int)v);
		printf("Invalid choice. Enter %d to %d ", low, high);
		puts("(blank to cancel).");
	}
}

/**
 * valid_device_id - check a device identifier
 * @s: identifier to check
 *
 * Return: 1 if @s has 1 to 31 letters, digits, '-' or '_', else 0
 */
int valid_device_id(const char *s)
{
	size_t n = strlen(s);

	if (n == 0 || n >= DEVICE_LEN)
		return (0);
	for (; *s != '\0'; s++)
	{
		if (!isalnum((unsigned char)*s) && *s != '-' && *s != '_')
			return (0);
	}
	return (1);
}

/**
 * prompt_device - ask for a valid device identifier
 * @device: buffer of DEVICE_LEN bytes that receives the answer
 *
 * Return: 1 if an identifier was entered, 0 if cancelled
 */
int prompt_device(char *device)
{
	char line[LINE_LEN], *s;
	int r;

	while (1)
	{
		printf("Sensor/device ID (blank to cancel): ");
		r = read_line(line, sizeof(line));
		s = trim(line);
		if (r < 0 || *s == '\0')
			return (0);
		if (r == 0 && valid_device_id(s))
		{
			snprintf(device, DEVICE_LEN, "%s", s);
			return (1);
		}
		printf("Invalid ID: use 1-%d letters, digits, - or _.\n",
				DEVICE_LEN - 1);
	}
}

/**
 * cmd_add - read a new event from the operator and add it
 * @lg: logger to add to
 */
void cmd_add(logger_t *lg)
{
	char device[DEVICE_LEN];
	int type, sev;
	event_t *ev;

	puts("Event types: 1) Temperature warning  2) Low battery");
	puts("             3) Sensor failure       4) Communication error");
	type = prompt_choice("Event type [1-4, blank to cancel]: ", 1, 4);
	if (type == 0 || !prompt_device(device))
	{
		puts("Add cancelled.");
		return;
	}
	puts("Severity: 1=LOW 2=MEDIUM 3=HIGH 4=CRITICAL");
	sev = prompt_choice("Severity [1-4, blank to cancel]: ", 1, 4);
	if (sev == 0)
	{
		puts("Add cancelled.");
		return;
	}
	ev = logger_add(lg, (event_type_t)type, device, (severity_t)sev,
			time(NULL));
	if (ev == NULL)
		puts("Error: out of memory - event not recorded.");
	else
		printf("Event #%u recorded (%d/%d in log).\n", ev->id,
				lg->count, MAX_EVENTS);
}
