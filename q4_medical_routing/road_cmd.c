#include "routing.h"

/**
 * ask_distance - prompt for a road length
 * @km: receives the number entered
 *
 * Return: 1 if a whole number was entered, 0 otherwise
 */
int ask_distance(long *km)
{
	char line[LINE_LEN], *s, *end;
	int r;

	printf("Distance in km: ");
	r = read_line(line, sizeof(line));
	s = trim(line);
	*km = strtol(s, &end, 10);
	if (r != 0 || *s == '\0' || *end != '\0')
	{
		puts("Invalid distance - enter a whole number of kilometres.");
		return (0);
	}
	return (1);
}

/**
 * report_road - tell the dispatcher what add-road did
 * @net: network that was changed
 * @a: index of the first location
 * @b: index of the second location
 * @km: distance that was entered
 * @res: result from net_add_road()
 */
void report_road(const network_t *net, int a, int b, long km,
		road_result_t res)
{
	switch (res)
	{
	case ROAD_OK:
		printf("Road added: %s - %s (%ld km).\n",
				net->names[a], net->names[b], km);
		break;
	case ROAD_UPDATED:
		printf("Road updated: %s - %s now %ld km.\n",
				net->names[a], net->names[b], km);
		break;
	case ROAD_SAME:
		puts("A road must connect two different locations.");
		break;
	case ROAD_BAD_DISTANCE:
		printf("Invalid distance %ld: use 1 to %d km.\n",
				km, MAX_ROAD_KM);
		break;
	default:
		puts("Invalid location.");
	}
}

/**
 * cmd_add_road - ask for two locations and a distance, add the road
 * @net: network to change
 */
void cmd_add_road(network_t *net)
{
	int a, b;
	long km;

	a = ask_location(net, "First location (name or number): ");
	if (a < 0)
		return;
	b = ask_location(net, "Second location (name or number): ");
	if (b < 0)
		return;
	if (!ask_distance(&km))
		return;
	report_road(net, a, b, km, net_add_road(net, a, b, km));
}

/**
 * cmd_add_location - ask for a name and add a new location
 * @net: network to change
 */
void cmd_add_location(network_t *net)
{
	char line[LINE_LEN], *s;
	int r;

	if (net->n >= MAX_LOC)
	{
		printf("The network is full (max %d locations).\n", MAX_LOC);
		return;
	}
	printf("New location name: ");
	r = read_line(line, sizeof(line));
	s = trim(line);
	if (r < 0 || *s == '\0')
		puts("No name entered - operation cancelled.");
	else if (r == 1 || strlen(s) >= NAME_LEN)
		printf("Name too long (max %d characters).\n", NAME_LEN - 1);
	else if (net_find(net, s) >= 0)
		printf("Location '%s' already exists.\n", s);
	else
	{
		net_add_location(net, s);
		printf("Location '%s' added (no roads yet).\n", s);
	}
}
