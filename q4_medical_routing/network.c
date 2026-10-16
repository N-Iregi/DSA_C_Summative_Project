#include "routing.h"

/**
 * net_add_location - add a location (a vertex) to the network
 * @net: network to change
 * @name: location name
 *
 * Return: index of the new location, or -1 if full or name too long
 */
int net_add_location(network_t *net, const char *name)
{
	if (net->n >= MAX_LOC || strlen(name) >= NAME_LEN)
		return (-1);
	snprintf(net->names[net->n], NAME_LEN, "%s", name);
	return (net->n++);
}

/**
 * net_find - find a location by name (case-insensitive)
 * @net: network to search
 * @name: location name
 *
 * Return: index of the location, or -1 if not found
 */
int net_find(const network_t *net, const char *name)
{
	int i;

	for (i = 0; i < net->n; i++)
	{
		if (ci_equal(net->names[i], name))
			return (i);
	}
	return (-1);
}

/**
 * net_add_road - connect two locations with a road
 * @net: network to change
 * @a: index of the first location
 * @b: index of the second location
 * @km: road length in kilometres (must be 1 to MAX_ROAD_KM)
 *
 * Return: a road_result_t describing what happened
 */
road_result_t net_add_road(network_t *net, int a, int b, long km)
{
	road_result_t res;

	if (a < 0 || b < 0 || a >= net->n || b >= net->n)
		return (ROAD_BAD_LOCATION);
	if (a == b)
		return (ROAD_SAME);
	if (km <= 0 || km > MAX_ROAD_KM)
		return (ROAD_BAD_DISTANCE);
	res = (net->dist[a][b] > 0) ? ROAD_UPDATED : ROAD_OK;
	net->dist[a][b] = (int)km;
	net->dist[b][a] = (int)km;
	return (res);
}

/**
 * build_network - load the six locations and nine roads
 * @net: network to fill
 */
void build_network(network_t *net)
{
	static const char * const places[] = {
		"Central Warehouse", "Distribution Center", "Health Center A",
		"Health Center B", "Health Center C", "District Hospital"
	};
	static const road_t roads[] = {
		{"Central Warehouse", "Distribution Center", 8},
		{"Central Warehouse", "District Hospital", 14},
		{"Distribution Center", "Health Center A", 5},
		{"Distribution Center", "Health Center B", 11},
		{"Health Center A", "District Hospital", 7},
		{"Health Center B", "Health Center C", 3},
		{"Health Center C", "District Hospital", 6},
		{"Health Center B", "District Hospital", 9},
		{"Health Center A", "Health Center C", 4}
	};
	size_t i;

	memset(net, 0, sizeof(network_t));
	for (i = 0; i < sizeof(places) / sizeof(places[0]); i++)
		net_add_location(net, places[i]);
	for (i = 0; i < sizeof(roads) / sizeof(roads[0]); i++)
		net_add_road(net, net_find(net, roads[i].a),
				net_find(net, roads[i].b), roads[i].km);
}
