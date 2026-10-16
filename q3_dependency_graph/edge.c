#include "graph.h"

/**
 * find_edge - look for the edge from -> to
 * @from: dependent server
 * @to: server it would depend on
 *
 * Return: the edge, or NULL if there is none
 */
edge_t *find_edge(const vertex_t *from, const vertex_t *to)
{
	edge_t *e;

	for (e = from->out; e != NULL; e = e->next)
	{
		if (e->to == to)
			return (e);
	}
	return (NULL);
}

/**
 * edge_remove - unlink and free the edge from -> to
 * @from: dependent server
 * @to: server it depends on
 *
 * Return: 1 if an edge was removed, 0 if there was none
 */
int edge_remove(vertex_t *from, const vertex_t *to)
{
	edge_t **pp, *dead;

	for (pp = &from->out; *pp != NULL; pp = &(*pp)->next)
	{
		if ((*pp)->to == to)
		{
			dead = *pp;
			*pp = dead->next;
			free(dead);
			return (1);
		}
	}
	return (0);
}

/**
 * graph_add_edge - record that one server depends on another
 * @g: graph to change
 * @from_name: the dependent server
 * @to_name: the server it depends on
 *
 * Return: G_OK, G_NOT_FOUND, G_SELF_LOOP, G_DUPLICATE or G_NOMEM
 */
graph_result_t graph_add_edge(graph_t *g, const char *from_name,
		const char *to_name)
{
	vertex_t *from, *to;
	edge_t *edge, **pp;

	from = find_vertex(g, from_name);
	to = find_vertex(g, to_name);
	if (from == NULL || to == NULL)
		return (G_NOT_FOUND);
	if (from == to)
		return (G_SELF_LOOP);
	if (find_edge(from, to) != NULL)
		return (G_DUPLICATE);
	edge = malloc(sizeof(edge_t));
	if (edge == NULL)
		return (G_NOMEM);
	edge->to = to;
	edge->next = NULL;
	pp = &from->out;
	while (*pp != NULL)
		pp = &(*pp)->next;
	*pp = edge;
	return (G_OK);
}
