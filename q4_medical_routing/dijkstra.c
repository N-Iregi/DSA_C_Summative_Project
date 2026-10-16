#include "routing.h"

/**
 * pick_closest - choose the unvisited vertex with the smallest distance
 * @net: network being searched
 * @dist: best known distances from the source
 * @visited: 1 for vertices already finalised
 *
 * Return: index of the closest reachable unvisited vertex, or -1
 */
static int pick_closest(const network_t *net, const int dist[],
		const int visited[])
{
	int i, u = -1;

	for (i = 0; i < net->n; i++)
	{
		if (visited[i] || dist[i] >= INF)
			continue;
		if (u == -1 || dist[i] < dist[u])
			u = i;
	}
	return (u);
}

/**
 * relax_edges - try to shorten the path to every neighbour of u
 * @net: network being searched
 * @u: vertex that has just been finalised
 * @visited: 1 for vertices already finalised
 * @dist: best known distances (updated)
 * @prev: predecessor of each vertex on its best path (updated)
 */
static void relax_edges(const network_t *net, int u, const int visited[],
		int dist[], int prev[])
{
	int v, alt;

	for (v = 0; v < net->n; v++)
	{
		if (net->dist[u][v] > 0 && !visited[v])
		{
			alt = dist[u] + net->dist[u][v];
			if (alt < dist[v])
			{
				dist[v] = alt;
				prev[v] = u;
			}
		}
	}
}

/**
 * dijkstra - shortest distances from src using Dijkstra's algorithm
 * @net: network to search
 * @src: start location index
 * @dst: destination index (the search stops once it is finalised)
 * @dist: receives the shortest distance to each vertex
 * @prev: receives each vertex's predecessor, to rebuild the route
 *
 * Return: 1 if @dst is reachable from @src, 0 otherwise
 */
int dijkstra(const network_t *net, int src, int dst, int dist[],
		int prev[])
{
	int visited[MAX_LOC] = {0};
	int i, u;

	for (i = 0; i < net->n; i++)
	{
		dist[i] = INF;
		prev[i] = -1;
	}
	dist[src] = 0;
	for (i = 0; i < net->n; i++)
	{
		u = pick_closest(net, dist, visited);
		if (u == -1)
			break;
		visited[u] = 1;
		if (u == dst)
			break;
		relax_edges(net, u, visited, dist, prev);
	}
	return (dist[dst] < INF);
}

/**
 * build_route - rebuild the path by following predecessors backwards
 * @route: receives the path
 * @prev: predecessor array from dijkstra()
 * @src: start location index
 * @dst: destination index
 * @total: total distance to store
 */
void build_route(route_t *route, const int prev[], int src, int dst,
		int total)
{
	int reversed[MAX_LOC], n = 0, v, i;

	for (v = dst; v != -1; v = prev[v])
		reversed[n++] = v;
	route->length = n;
	for (i = 0; i < n; i++)
		route->path[i] = reversed[n - 1 - i];
	route->total = total;
	route->valid = (route->path[0] == src);
}
