#include "graph.h"

/**
 * graph_free - free every edge and vertex
 * @g: graph to empty
 */
void graph_free(graph_t *g)
{
	vertex_t *v, *vnext;
	edge_t *e, *enext;

	for (v = g->head; v != NULL; v = vnext)
	{
		vnext = v->next;
		for (e = v->out; e != NULL; e = enext)
		{
			enext = e->next;
			free(e);
		}
		free(v);
	}
	g->head = NULL;
	g->count = 0;
}

/**
 * build_initial_graph - load the six servers and seven dependencies
 * @g: empty graph to fill
 */
void build_initial_graph(graph_t *g)
{
	static const char * const servers[] = {
		"Web Server", "API Server", "Authentication Server",
		"Database Server", "Cache Server", "Backup Server"
	};
	static const char * const edges[][2] = {
		{"Web Server", "API Server"},
		{"Web Server", "Authentication Server"},
		{"API Server", "Database Server"},
		{"API Server", "Cache Server"},
		{"Authentication Server", "Database Server"},
		{"Cache Server", "Database Server"},
		{"Database Server", "Backup Server"}
	};
	size_t i;

	for (i = 0; i < sizeof(servers) / sizeof(servers[0]); i++)
		graph_add_vertex(g, servers[i]);
	for (i = 0; i < sizeof(edges) / sizeof(edges[0]); i++)
		graph_add_edge(g, edges[i][0], edges[i][1]);
}
