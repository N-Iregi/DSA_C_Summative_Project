#ifndef ROUTING_H
#define ROUTING_H

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOC 16
#define NAME_LEN 32
#define LINE_LEN 128
#define INF (INT_MAX / 2)
#define MAX_ROAD_KM 100000
#define ARROW " \xe2\x86\x92 "

/**
 * struct network_s - the road network (weighted, undirected graph)
 * @names: location names
 * @n: number of locations
 * @dist: dist[a][b] is the road length in km, 0 means no road
 */
typedef struct network_s
{
	char names[MAX_LOC][NAME_LEN];
	int n;
	int dist[MAX_LOC][MAX_LOC];
} network_t;

/**
 * struct route_s - the last route that was calculated
 * @valid: 1 if a route is stored
 * @path: location indexes from start to destination
 * @length: number of locations in @path
 * @total: total distance in km
 */
typedef struct route_s
{
	int valid;
	int path[MAX_LOC];
	int length;
	int total;
} route_t;

/**
 * struct road_s - one road in the built-in network table
 * @a: name of the first location
 * @b: name of the second location
 * @km: road length in kilometres
 */
typedef struct road_s
{
	const char *a;
	const char *b;
	int km;
} road_t;

/**
 * enum road_result_e - outcome of adding a road
 * @ROAD_OK: new road added
 * @ROAD_UPDATED: an existing road got a new length
 * @ROAD_BAD_LOCATION: a location index is invalid
 * @ROAD_SAME: both ends are the same location
 * @ROAD_BAD_DISTANCE: distance is zero, negative or too large
 */
typedef enum road_result_e
{
	ROAD_OK,
	ROAD_UPDATED,
	ROAD_BAD_LOCATION,
	ROAD_SAME,
	ROAD_BAD_DISTANCE
} road_result_t;

/* input.c */
int read_line(char *buf, size_t size);
char *trim(char *s);
int ci_equal(const char *a, const char *b);

/* network.c */
int net_add_location(network_t *net, const char *name);
int net_find(const network_t *net, const char *name);
road_result_t net_add_road(network_t *net, int a, int b, long km);
void build_network(network_t *net);

/* dijkstra.c */
int dijkstra(const network_t *net, int src, int dst, int dist[],
		int prev[]);
void build_route(route_t *route, const int prev[], int src, int dst,
		int total);

/* display.c */
void show_locations(const network_t *net);
void print_route(const network_t *net, const route_t *route);
void cmd_show_distance(const network_t *net, const route_t *route);

/* route_cmd.c */
int resolve_location(const network_t *net, const char *text);
int ask_location(const network_t *net, const char *prompt);
void cmd_find_route(const network_t *net, route_t *route);

/* road_cmd.c */
int ask_distance(long *km);
void report_road(const network_t *net, int a, int b, long km,
		road_result_t res);
void cmd_add_road(network_t *net);
void cmd_add_location(network_t *net);

/* main.c */
void print_menu(void);
void run_choice(network_t *net, route_t *route, long choice);

#endif /* ROUTING_H */
