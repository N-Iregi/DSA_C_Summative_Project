#include "routing.h"

/**
 * show_locations - list every location and its roads
 * @net: network to display
 */
void show_locations(const network_t *net)
{
	int i, j;

	printf("\nLocations (%d):\n", net->n);
	for (i = 0; i < net->n; i++)
	{
		printf("  %d. %s\n", i + 1, net->names[i]);
		for (j = 0; j < net->n; j++)
		{
			if (net->dist[i][j] > 0)
				printf("       road to %-20s %3d km\n",
						net->names[j], net->dist[i][j]);
		}
	}
}

/**
 * print_route - print a route and its total distance
 * @net: network the route belongs to
 * @route: route to print
 */
void print_route(const network_t *net, const route_t *route)
{
	int i;

	printf("\nShortest Route:\n\n");
	for (i = 0; i < route->length; i++)
		printf("%s%s", i ? ARROW : "", net->names[route->path[i]]);
	printf("\n\nTotal Distance: %d km\n", route->total);
}

/**
 * cmd_show_distance - show the last route leg by leg
 * @net: network the route belongs to
 * @route: last calculated route
 */
void cmd_show_distance(const network_t *net, const route_t *route)
{
	int i, a, b;

	if (!route->valid)
	{
		puts("No route calculated yet - use option 2 first.");
		return;
	}
	printf("\nLast route: %s to %s\n", net->names[route->path[0]],
			net->names[route->path[route->length - 1]]);
	for (i = 0; i + 1 < route->length; i++)
	{
		a = route->path[i];
		b = route->path[i + 1];
		printf("  %-20s to %-20s %3d km\n", net->names[a],
				net->names[b], net->dist[a][b]);
	}
	printf("Total distance: %d km (%d road(s))\n", route->total,
			route->length - 1);
}
