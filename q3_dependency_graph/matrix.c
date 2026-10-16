#include "graph.h"

/**
 * index_of - position of a vertex in the order array
 * @order: array of vertices in list order
 * @n: number of vertices
 * @v: vertex to find
 *
 * Return: index of @v, or -1
 */
static int index_of(vertex_t **order, int n, const vertex_t *v)
{
	int i;

	for (i = 0; i < n; i++)
	{
		if (order[i] == v)
			return (i);
	}
	return (-1);
}

/**
 * fill_matrix - set matrix[A][B] = 1 for every edge A -> B
 * @g: graph to read
 * @order: array that receives the vertices in list order
 * @matrix: n x n array of zeros that receives the edges
 */
static void fill_matrix(const graph_t *g, vertex_t **order, int *matrix)
{
	int n = g->count, i = 0, j;
	vertex_t *v;
	edge_t *e;

	for (v = g->head; v != NULL; v = v->next)
		order[i++] = v;
	for (i = 0; i < n; i++)
	{
		for (e = order[i]->out; e != NULL; e = e->next)
		{
			j = index_of(order, n, e->to);
			matrix[i * n + j] = 1;
		}
	}
}

/**
 * print_rows - print the labelled matrix
 * @order: vertices in list order
 * @matrix: n x n adjacency matrix
 * @n: number of vertices
 */
static void print_rows(vertex_t **order, const int *matrix, int n)
{
	size_t width = 0, w;
	char label[NAME_LEN + 16];
	int i, j;

	for (i = 0; i < n; i++)
	{
		w = strlen(order[i]->name);
		if (w > width)
			width = w;
	}
	width += 6;
	printf("\nAdjacency matrix (row depends on column):\n\n");
	printf("%-*s", (int)width, "");
	for (j = 0; j < n; j++)
		printf("%3d", j);
	putchar('\n');
	for (i = 0; i < n; i++)
	{
		snprintf(label, sizeof(label), "[%d] %s", i, order[i]->name);
		printf("%-*s", (int)width, label);
		for (j = 0; j < n; j++)
			printf("%3d", matrix[i * n + j]);
		putchar('\n');
	}
}

/**
 * graph_print_matrix - build the adjacency matrix from the live graph
 * @g: graph to display
 */
void graph_print_matrix(const graph_t *g)
{
	vertex_t **order;
	int *matrix, n = g->count;

	if (n == 0)
	{
		puts("The graph is empty - no adjacency matrix to display.");
		return;
	}
	order = malloc((size_t)n * sizeof(vertex_t *));
	matrix = calloc((size_t)n * (size_t)n, sizeof(int));
	if (order == NULL || matrix == NULL)
		puts("Error: out of memory while building the matrix.");
	else
	{
		fill_matrix(g, order, matrix);
		print_rows(order, matrix, n);
	}
	free(matrix);
	free(order);
}
