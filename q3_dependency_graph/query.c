#include "graph.h"

/**
 * print_dependents - list the servers that depend on a server
 * @g: graph to scan
 * @v: server being asked about
 */
void print_dependents(const graph_t *g, const vertex_t *v)
{
	const vertex_t *u;
	int found = 0;

	printf("\nServices depending on %s:\n", v->name);
	for (u = g->head; u != NULL; u = u->next)
	{
		if (find_edge(u, v) != NULL)
		{
			printf("  %s\n", u->name);
			found = 1;
		}
	}
	if (!found)
		puts("  (none)");
}

/**
 * graph_query - show what a server depends on and what depends on it
 * @g: graph to query
 * @name: server name
 */
void graph_query(const graph_t *g, const char *name)
{
	const vertex_t *v;
	const edge_t *e;

	v = find_vertex(g, name);
	if (v == NULL)
	{
		printf("Server '%s' does not exist.\n", name);
		return;
	}
	printf("\nServer: %s\n\nDirect dependencies:\n", v->name);
	if (v->out == NULL)
		puts("  (none)");
	for (e = v->out; e != NULL; e = e->next)
		printf("  %s\n", e->to->name);
	print_dependents(g, v);
}

/**
 * cmd_query - ask for a server name and query it
 * @g: graph to query
 */
void cmd_query(const graph_t *g)
{
	char name[NAME_LEN];

	if (g->count == 0)
	{
		puts("The graph is empty - there are no servers to query.");
		return;
	}
	if (prompt_name("Server name: ", name, sizeof(name)))
		graph_query(g, name);
}
