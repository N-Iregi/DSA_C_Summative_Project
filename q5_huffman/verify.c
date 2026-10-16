#include "huffman.h"

/**
 * files_equal - compare two open files byte by byte
 * @a: first file
 * @b: second file
 *
 * Return: 1 if they have identical contents, 0 otherwise
 */
int files_equal(FILE *a, FILE *b)
{
	int ca, cb;

	while (1)
	{
		ca = fgetc(a);
		cb = fgetc(b);
		if (ca != cb)
			return (0);
		if (ca == EOF)
			return (1);
	}
}

/**
 * do_verify - compare the original log with the restored one
 */
void do_verify(void)
{
	FILE *a, *b;
	int same;

	a = fopen(INPUT_FILE, "rb");
	b = fopen(RESTORED_FILE, "rb");
	if (a == NULL || b == NULL)
	{
		if (a == NULL)
			printf("Cannot open '%s' (%s).\n", INPUT_FILE,
					strerror(errno));
		if (b == NULL)
			printf("Cannot open '%s' - decompress first.\n",
					RESTORED_FILE);
		if (a != NULL)
			fclose(a);
		if (b != NULL)
			fclose(b);
		return;
	}
	same = files_equal(a, b);
	fclose(a);
	fclose(b);
	puts(same ? "Verification: SUCCESS" : "Verification: FAILED");
	if (same)
		puts("The restored file is identical to the original.");
	else
		puts("The restored file differs from the original.");
}
