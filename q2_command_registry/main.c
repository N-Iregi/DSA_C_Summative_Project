#include "registry.h"

/**
 * run_command - execute one single-letter command
 * @reg: registry to operate on
 * @stats: verification counters
 * @cmd: lower-case command letter
 */
void run_command(registry_t *reg, stats_t *stats, char cmd)
{
	switch (cmd)
	{
	case 'v':
		cmd_verify(reg, stats);
		break;
	case 'p':
		cmd_print(reg);
		break;
	case 's':
		cmd_count(reg);
		break;
	case 'x':
		cmd_clear(reg);
		break;
	default:
		puts("Invalid command. Choose v, p, s, x or q.");
	}
}

/**
 * main - command registry loop
 * @argc: argument count
 * @argv: argument vector; argv[1] is the optional command file
 *
 * Return: always 0
 */
int main(int argc, char **argv)
{
	const char *path = (argc > 1) ? argv[1] : DEFAULT_FILE;
	registry_t reg = {NULL, 0};
	stats_t stats = {0, 0, 0};
	char line[LINE_LEN], *s;
	int r;

	puts("Network Device Command Registry");
	if (load_commands(&reg, path) < 0)
		printf("Error: cannot open '%s' (%s). Registry is empty.\n",
				path, strerror(errno));
	print_help();
	while (1)
	{
		printf("\nCommand [v/p/s/x/q]> ");
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
			puts("Invalid command. Choose v, p, s, x or q.");
			continue;
		}
		if (tolower((unsigned char)s[0]) == 'q')
			break;
		run_command(&reg, &stats, (char)tolower((unsigned char)s[0]));
	}
	save_session(&reg, &stats);
	registry_clear(&reg);
	puts("Goodbye.");
	return (0);
}
