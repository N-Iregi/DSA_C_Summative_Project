#include "graph.h"

/**
 * print_help - list the available commands
 */
void print_help(void)
{
	puts("Commands:");
	puts("  a  add server            r  remove server");
	puts("  c  create dependency     d  delete dependency");
	puts("  q  query server          m  display adjacency matrix");
	puts("  x  exit                  h  help");
	puts("A dependency 'A -> B' means A depends on B.");
}

/**
 * run_command - execute one single-letter command
 * @g: graph to operate on
 * @cmd: lower-case command letter
 */
void run_command(graph_t *g, char cmd)
{
	switch (cmd)
	{
	case 'a':
		cmd_add_server(g);
		break;
	case 'r':
		cmd_remove_server(g);
		break;
	case 'c':
		cmd_create_dependency(g);
		break;
	case 'd':
		cmd_delete_dependency(g);
		break;
	case 'q':
		cmd_query(g);
		break;
	case 'm':
		graph_print_matrix(g);
		break;
	case 'h':
		print_help();
		break;
	default:
		puts("Invalid command. Type 'h' for help.");
	}
}

/**
 * main - dependency analyzer command loop
 *
 * Return: always 0
 */
int main(void)
{
	graph_t g = {NULL, 0};
	char line[LINE_LEN], *s;
	int r;

	build_initial_graph(&g);
	printf("Server Dependency Network Analyzer (%d servers)\n", g.count);
	print_help();
	while (1)
	{
		printf("\nCommand [a/r/c/d/q/m/x]> ");
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
		if (tolower((unsigned char)s[0]) == 'x')
			break;
		run_command(&g, (char)tolower((unsigned char)s[0]));
	}
	graph_free(&g);
	puts("Goodbye.");
	return (0);
}
