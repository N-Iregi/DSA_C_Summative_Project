#include "graph.h"

/**
 * find_vertex - look a server up by name (case-insensitive)
 * @g: graph to search
 * @name: server name
 *
 * Return: the vertex, or NULL if it does not exist
 */
vertex_t *find_vertex(const graph_t *g, const char *name)
{
	vertex_t *v;

	for (v = g->head; v != NULL; v = v->next)
	{
		if (ci_equal(v->name, name))
			return (v);
	}
	return (NULL);
}

/**
 * graph_add_vertex - add a new server at the end of the vertex list
 * @g: graph to add to
 * @name: server name
 *
 * Return: G_OK, G_EMPTY_NAME, G_NAME_TOO_LONG, G_DUPLICATE or G_NOMEM
 */
graph_result_t graph_add_vertex(graph_t *g, const char *name)
{
	vertex_t *v, **pp;

	if (*name == '\0')
		return (G_EMPTY_NAME);
	if (strlen(name) >= NAME_LEN)
		return (G_NAME_TOO_LONG);
	if (find_vertex(g, name) != NULL)
		return (G_DUPLICATE);
	v = calloc(1, sizeof(vertex_t));
	if (v == NULL)
		return (G_NOMEM);
	snprintf(v->name, sizeof(v->name), "%s", name);
	pp = &g->head;
	while (*pp != NULL)
		pp = &(*pp)->next;
	*pp = v;
	g->count++;
	return (G_OK);
}

/**
 * graph_remove_vertex - delete a server and every edge touching it
 * @g: graph to change
 * @name: server to remove
 * @edges_removed: receives the number of edges deleted
 *
 * Return: G_OK, or G_NOT_FOUND if the server does not exist
 */
graph_result_t graph_remove_vertex(graph_t *g, const char *name,
		int *edges_removed)
{
	vertex_t *victim, *u, **pp;
	edge_t *e, *next;

	*edges_removed = 0;
	victim = find_vertex(g, name);
	if (victim == NULL)
		return (G_NOT_FOUND);
	for (u = g->head; u != NULL; u = u->next)
	{
		if (u != victim && edge_remove(u, victim))
			(*edges_removed)++;
	}
	for (e = victim->out; e != NULL; e = next)
	{
		next = e->next;
		free(e);
		(*edges_removed)++;
	}
	pp = &g->head;
	while (*pp != victim)
		pp = &(*pp)->next;
	*pp = victim->next;
	free(victim);
	g->count--;
	return (G_OK);
}
