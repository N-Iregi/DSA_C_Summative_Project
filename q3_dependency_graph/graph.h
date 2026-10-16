#ifndef GRAPH_H
#define GRAPH_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 64
#define LINE_LEN 256

/**
 * struct edge_s - one directed dependency (adjacency-list node)
 * @to: the server that is depended upon
 * @next: next dependency of the same server
 */
typedef struct edge_s
{
	struct vertex_s *to;
	struct edge_s *next;
} edge_t;

/**
 * struct vertex_s - one server
 * @name: server name
 * @out: list of servers this one depends on
 * @next: next vertex in the graph's vertex list
 */
typedef struct vertex_s
{
	char name[NAME_LEN];
	edge_t *out;
	struct vertex_s *next;
} vertex_t;

/**
 * struct graph_s - the server dependency graph
 * @head: first vertex
 * @count: number of vertices
 */
typedef struct graph_s
{
	vertex_t *head;
	int count;
} graph_t;

/**
 * enum graph_result_e - outcome of a graph operation
 * @G_OK: done
 * @G_EMPTY_NAME: name was empty
 * @G_NAME_TOO_LONG: name does not fit in NAME_LEN
 * @G_DUPLICATE: server or dependency already exists
 * @G_NOT_FOUND: a server does not exist
 * @G_SELF_LOOP: a server cannot depend on itself
 * @G_NOMEM: malloc failed
 */
typedef enum graph_result_e
{
	G_OK,
	G_EMPTY_NAME,
	G_NAME_TOO_LONG,
	G_DUPLICATE,
	G_NOT_FOUND,
	G_SELF_LOOP,
	G_NOMEM
} graph_result_t;

/* input.c */
int read_line(char *buf, size_t size);
char *trim(char *s);
int ci_equal(const char *a, const char *b);
int prompt_name(const char *prompt, char *out, size_t size);

/* vertex.c */
vertex_t *find_vertex(const graph_t *g, const char *name);
graph_result_t graph_add_vertex(graph_t *g, const char *name);
graph_result_t graph_remove_vertex(graph_t *g, const char *name,
		int *edges_removed);

/* edge.c */
edge_t *find_edge(const vertex_t *from, const vertex_t *to);
int edge_remove(vertex_t *from, const vertex_t *to);
graph_result_t graph_add_edge(graph_t *g, const char *from_name,
		const char *to_name);

/* init.c */
void graph_free(graph_t *g);
void build_initial_graph(graph_t *g);

/* query.c */
void print_dependents(const graph_t *g, const vertex_t *v);
void graph_query(const graph_t *g, const char *name);
void cmd_query(const graph_t *g);

/* matrix.c */
void graph_print_matrix(const graph_t *g);

/* commands.c */
void report_missing(const graph_t *g, const char *from, const char *to);
void cmd_add_server(graph_t *g);
void cmd_remove_server(graph_t *g);
void cmd_create_dependency(graph_t *g);
void cmd_delete_dependency(graph_t *g);

/* main.c */
void print_help(void);
void run_command(graph_t *g, char cmd);

#endif /* GRAPH_H */
