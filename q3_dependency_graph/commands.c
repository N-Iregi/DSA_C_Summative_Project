#include "graph.h"

/**
 * report_missing - say which of two servers do not exist
 * @g: graph to check
 * @from: first server name
 * @to: second server name
 */
void report_missing(const graph_t *g, const char *from, const char *to)
{
	if (find_vertex(g, from) == NULL)
		printf("Server '%s' does not exist.\n", from);
	if (find_vertex(g, to) == NULL)
		printf("Server '%s' does not exist.\n", to);
}

/**
 * cmd_add_server - ask for a name and add that server
 * @g: graph to change
 */
void cmd_add_server(graph_t *g)
{
	char name[NAME_LEN];

	if (!prompt_name("New server name: ", name, sizeof(name)))
		return;
	switch (graph_add_vertex(g, name))
	{
	case G_OK:
		printf("Server '%s' added.\n", name);
		break;
	case G_DUPLICATE:
		printf("Server '%s' already exists - not added.\n", name);
		break;
	case G_NOMEM:
		puts("Error: out of memory.");
		break;
	default:
		puts("Invalid server name.");
	}
}

/**
 * cmd_remove_server - ask for a name and remove that server
 * @g: graph to change
 */
void cmd_remove_server(graph_t *g)
{
	char name[NAME_LEN];
	int removed;

	if (g->count == 0)
	{
		puts("The graph is empty - nothing to remove.");
		return;
	}
	if (!prompt_name("Server to remove: ", name, sizeof(name)))
		return;
	if (graph_remove_vertex(g, name, &removed) == G_OK)
		printf("Server '%s' removed with %d dependency link(s).\n",
				name, removed);
	else
		printf("Server '%s' does not exist - nothing removed.\n", name);
}

/**
 * cmd_create_dependency - ask for two servers and link them
 * @g: graph to change
 */
void cmd_create_dependency(graph_t *g)
{
	char from[NAME_LEN], to[NAME_LEN];

	if (g->count == 0)
	{
		puts("The graph is empty - add servers first.");
		return;
	}
	if (!prompt_name("Dependent server: ", from, sizeof(from)) ||
			!prompt_name("Server it depends on: ", to, sizeof(to)))
		return;
	switch (graph_add_edge(g, from, to))
	{
	case G_OK:
		printf("Dependency added: %s -> ", find_vertex(g, from)->name);
		printf("%s\n", find_vertex(g, to)->name);
		break;
	case G_NOT_FOUND:
		report_missing(g, from, to);
		puts("Dependency not created.");
		break;
	case G_SELF_LOOP:
		puts("A server cannot depend on itself.");
		break;
	case G_DUPLICATE:
		puts("That dependency already exists - not added.");
		break;
	default:
		puts("Error: out of memory.");
	}
}

/**
 * cmd_delete_dependency - ask for two servers and unlink them
 * @g: graph to change
 */
void cmd_delete_dependency(graph_t *g)
{
	char from[NAME_LEN], to[NAME_LEN];
	vertex_t *a, *b;

	if (g->count == 0)
	{
		puts("The graph is empty - no dependencies to delete.");
		return;
	}
	if (!prompt_name("Dependent server: ", from, sizeof(from)) ||
			!prompt_name("Server it depends on: ", to, sizeof(to)))
		return;
	a = find_vertex(g, from);
	b = find_vertex(g, to);
	if (a == NULL || b == NULL)
	{
		report_missing(g, from, to);
		puts("Nothing deleted.");
	}
	else if (edge_remove(a, b))
		printf("Dependency removed: %s -> %s\n", a->name, b->name);
	else
		printf("No dependency from '%s' to '%s'.\n", a->name, b->name);
}
