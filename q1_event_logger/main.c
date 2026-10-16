#include "event_logger.h"

/**
 * print_help - list the available commands
 */
void print_help(void)
{
	puts("Commands:");
	puts("  n  next (newer) event      p  previous (older) event");
	puts("  a  add a new event         c  display current event");
	puts("  x  clear all events        q  save session and quit");
	puts("  l  list all events         h  show this help");
}

/**
 * run_command - execute one single-letter command
 * @lg: logger to operate on
 * @cmd: lower-case command letter
 */
void run_command(logger_t *lg, char cmd)
{
	switch (cmd)
	{
	case 'n':
		cmd_next(lg);
		break;
	case 'p':
		cmd_prev(lg);
		break;
	case 'a':
		cmd_add(lg);
		break;
	case 'c':
		cmd_show(lg);
		break;
	case 'x':
		cmd_clear(lg);
		break;
	case 'l':
		cmd_list(lg);
		break;
	case 'h':
		print_help();
		break;
	default:
		puts("Invalid command. Type 'h' for help.");
	}
}

/**
 * main - event logger command loop
 *
 * Return: always 0
 */
int main(void)
{
	logger_t lg;
	char line[LINE_LEN], *s;
	int r;

	logger_init(&lg);
	printf("Embedded Event Logger (capacity %d events)\n", MAX_EVENTS);
	print_help();
	while (1)
	{
		printf("\nCommand [n/p/a/c/x/q]> ");
		r = read_line(line, sizeof(line));
		if (r < 0)
		{
			putchar('\n');
			break;
		}
		s = trim(line);
		if (*s == '\0')
			continue;
		if (r != 0 || s[1] != '\0')
		{
			puts("Invalid command. Type 'h' for help.");
			continue;
		}
		if (tolower((unsigned char)s[0]) == 'q')
			break;
		run_command(&lg, (char)tolower((unsigned char)s[0]));
	}
	end_session(&lg);
	return (0);
}
