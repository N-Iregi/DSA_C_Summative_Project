#include "registry.h"

/**
 * add_line - validate one line from the file and insert it
 * @reg: registry to add to
 * @line: raw line text
 * @st: counters updated when the line is rejected
 */
static void add_line(registry_t *reg, char *line, load_stats_t *st)
{
	char *cmd = trim(line);

	if (*cmd == '\0')
		return;
	if (strlen(cmd) > MAX_CMD_LEN)
	{
		st->too_long++;
		return;
	}
	if (!is_printable_text(cmd))
	{
		st->bad_chars++;
		return;
	}
	switch (registry_insert(reg, cmd))
	{
	case INS_DUPLICATE:
		st->duplicates++;
		break;
	case INS_FULL:
		st->over_limit++;
		break;
	case INS_NOMEM:
		st->nomem++;
		break;
	default:
		break;
	}
}

/**
 * read_commands - read the file line by line into the tree
 * @reg: registry to fill
 * @fp: open command file
 * @st: counters for rejected lines
 */
static void read_commands(registry_t *reg, FILE *fp, load_stats_t *st)
{
	char line[LINE_LEN];
	size_t len;
	int c;

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		len = strlen(line);
		if ((len == 0 || line[len - 1] != '\n') && !feof(fp))
		{
			while ((c = fgetc(fp)) != '\n' && c != EOF)
				continue;
			st->too_long++;
			continue;
		}
		add_line(reg, line, st);
	}
}

/**
 * report_load - tell the technician what happened while loading
 * @reg: registry that was filled
 * @path: file that was read
 * @st: counters for rejected lines
 */
static void report_load(const registry_t *reg, const char *path,
		const load_stats_t *st)
{
	printf("Loaded %d approved command(s) from '%s'.\n", reg->count, path);
	if (reg->count == 0)
		puts("  Note: no usable commands - the registry is empty.");
	if (st->duplicates)
		printf("  Skipped %d duplicate command(s).\n", st->duplicates);
	if (st->over_limit)
		printf("  Limit of %d reached: %d command(s) NOT loaded.\n",
				MAX_COMMANDS, st->over_limit);
	if (st->too_long)
		printf("  Skipped %d line(s) over %d characters.\n",
				st->too_long, MAX_CMD_LEN);
	if (st->bad_chars)
		printf("  Skipped %d line(s) with non-printable characters.\n",
				st->bad_chars);
	if (st->nomem)
		printf("  Out of memory: %d not loaded.\n", st->nomem);
}

/**
 * load_commands - load approved commands from a text file
 * @reg: registry to fill
 * @path: path of the command file
 *
 * Return: -1 if the file cannot be opened, else the number loaded
 */
int load_commands(registry_t *reg, const char *path)
{
	FILE *fp;
	load_stats_t st = {0, 0, 0, 0, 0};

	fp = fopen(path, "r");
	if (fp == NULL)
		return (-1);
	read_commands(reg, fp, &st);
	fclose(fp);
	report_load(reg, path, &st);
	return (reg->count);
}
