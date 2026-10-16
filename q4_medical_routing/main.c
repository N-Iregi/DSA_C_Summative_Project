#include "routing.h"

/**
 * print_menu - show the dispatcher menu
 */
void print_menu(void)
{
	puts("\n--- Menu ---");
	puts("1. Display locations");
	puts("2. Find shortest route");
	puts("3. Display route distance");
	puts("4. Exit");
	puts("5. Add or update a road");
	puts("6. Add a location");
}

/**
 * run_choice - carry out one menu choice
 * @net: network to operate on
 * @route: last calculated route
 * @choice: number the dispatcher entered
 */
void run_choice(network_t *net, route_t *route, long choice)
{
	switch (choice)
	{
	case 1:
		show_locations(net);
		break;
	case 2:
		cmd_find_route(net, route);
		break;
	case 3:
		cmd_show_distance(net, route);
		break;
	case 5:
		cmd_add_road(net);
		route->valid = 0;
		break;
	case 6:
		cmd_add_location(net);
		break;
	default:
		puts("Invalid choice. Enter a number from 1 to 6.");
	}
}

/**
 * main - emergency medical supply routing menu
 *
 * Return: always 0
 */
int main(void)
{
	network_t net;
	route_t route = {0, {0}, 0, 0};
	char line[LINE_LEN], *s, *end;
	long choice;
	int r;

	build_network(&net);
	puts("Emergency Medical Supply Routing");
	while (1)
	{
		print_menu();
		printf("Choice: ");
		r = read_line(line, sizeof(line));
		if (r < 0)
		{
			putchar('\n');
			break;
		}
		s = trim(line);
		choice = strtol(s, &end, 10);
		if (r != 0 || *s == '\0' || *end != '\0')
		{
			puts("Invalid choice. Enter a number from 1 to 6.");
			continue;
		}
		if (choice == 4)
			break;
		run_choice(&net, &route, choice);
	}
	puts("Goodbye.");
	return (0);
}
