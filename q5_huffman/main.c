#include "huffman.h"

/**
 * print_menu - show the menu
 */
void print_menu(void)
{
	puts("\n--- Menu ---");
	puts("1. Compress log");
	puts("2. Decompress log");
	puts("3. Verify restored log");
	puts("4. Exit");
}

/**
 * run_choice - carry out one menu choice
 * @choice: number the user entered
 */
void run_choice(long choice)
{
	switch (choice)
	{
	case 1:
		do_compress();
		break;
	case 2:
		do_decompress();
		break;
	case 3:
		do_verify();
		break;
	default:
		puts("Invalid choice. Enter 1, 2, 3 or 4.");
	}
}

/**
 * main - Huffman log compression menu
 *
 * Return: always 0
 */
int main(void)
{
	char line[LINE_LEN], *s, *end;
	long choice;
	int r;

	puts("Embedded Firmware Log Compression Utility (Huffman coding)");
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
			puts("Invalid choice. Enter 1, 2, 3 or 4.");
			continue;
		}
		if (choice == 4)
			break;
		run_choice(choice);
	}
	puts("Goodbye.");
	return (0);
}
