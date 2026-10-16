#include "registry.h"

/**
 * print_help - list the available commands
 */
void print_help(void)
{
	puts("Commands:");
	puts("  v  verify a device command");
	puts("  p  print all approved commands (alphabetical)");
	puts("  s  show number of stored commands");
	puts("  x  clear the command registry");
	puts("  q  save session and quit");
}

/**
 * cmd_print - print all commands using an in-order traversal
 * @reg: registry to print
 */
void cmd_print(const registry_t *reg)
{
	int index = 0;

	if (reg->root == NULL)
	{
		puts("The registry is empty.");
		return;
	}
	printf("Approved commands (%d):\n", reg->count);
	tree_print_inorder(reg->root, &index);
}

/**
 * cmd_count - show how many commands are stored
 * @reg: registry to count
 */
void cmd_count(const registry_t *reg)
{
	printf("Stored commands: %d (maximum %d)\n", reg->count, MAX_COMMANDS);
}

/**
 * cmd_clear - free every node in the registry
 * @reg: registry to clear
 */
void cmd_clear(registry_t *reg)
{
	if (reg->count == 0)
	{
		puts("The registry is already empty.");
		return;
	}
	printf("Registry cleared: %d command(s) removed, memory released.\n",
			registry_clear(reg));
}
