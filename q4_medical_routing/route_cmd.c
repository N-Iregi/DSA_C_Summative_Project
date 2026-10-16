#include "routing.h"

/**
 * resolve_location - turn text into a location index
 * @net: network to search
 * @text: a list number (1-based) or a location name
 *
 * Return: index of the location, or -1 if it does not exist
 */
int resolve_location(const network_t *net, const char *text)
{
	char *end;
	long v;

	v = strtol(text, &end, 10);
	if (*end == '\0' && end != text)
		return ((v >= 1 && v <= net->n) ? (int)(v - 1) : -1);
	return (net_find(net, text));
}

/**
 * ask_location - prompt for a location
 * @net: network to search
 * @prompt: text shown to the dispatcher
 *
 * Return: index of the location, -1 if it does not exist, or -2 if
 * nothing usable was entered
 */
int ask_location(const network_t *net, const char *prompt)
{
	char line[LINE_LEN], *s;
	int r, idx;

	printf("%s", prompt);
	r = read_line(line, sizeof(line));
	s = trim(line);
	if (r != 0 || *s == '\0')
	{
		puts("Invalid input - no location entered.");
		return (-2);
	}
	idx = resolve_location(net, s);
	if (idx < 0)
		printf("Location '%s' does not exist.\n", s);
	return (idx);
}

/**
 * cmd_find_route - ask for two locations and show the shortest route
 * @net: network to search
 * @route: receives the route (route->valid is 0 on failure)
 */
void cmd_find_route(const network_t *net, route_t *route)
{
	int src, dst, dist[MAX_LOC], prev[MAX_LOC];

	route->valid = 0;
	show_locations(net);
	putchar('\n');
	src = ask_location(net, "Start location (name or number): ");
	if (src < 0)
		return;
	dst = ask_location(net, "Destination (name or number): ");
	if (dst < 0)
		return;
	if (src == dst)
	{
		printf("Start and destination are both '%s'.\n",
				net->names[src]);
		puts("Choose two different locations.");
		return;
	}
	if (!dijkstra(net, src, dst, dist, prev))
	{
		printf("No route exists from %s to %s - unreachable.\n",
				net->names[src], net->names[dst]);
		return;
	}
	build_route(route, prev, src, dst, dist[dst]);
	if (route->valid)
		print_route(net, route);
	else
		puts("Internal error: could not reconstruct the route.");
}
