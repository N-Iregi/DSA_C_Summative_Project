#include "huffman.h"

/**
 * build_codes - walk the tree and record each leaf's bit string
 * @n: node to visit
 * @buf: scratch buffer holding the path so far
 * @depth: current depth (length of the path)
 * @codes: receives a malloc'ed string of '0'/'1' per symbol
 *
 * A left edge is '0' and a right edge is '1'. A tree with a single
 * leaf gets the code "0" so that every symbol still costs one bit.
 *
 * Return: 1 on success, 0 if malloc failed
 */
int build_codes(const huff_node_t *n, char *buf, int depth, char *codes[])
{
	if (is_leaf(n))
	{
		if (depth == 0)
		{
			buf[0] = '0';
			depth = 1;
		}
		buf[depth] = '\0';
		codes[n->symbol] = malloc((size_t)depth + 1);
		if (codes[n->symbol] == NULL)
			return (0);
		memcpy(codes[n->symbol], buf, (size_t)depth + 1);
		return (1);
	}
	buf[depth] = '0';
	if (!build_codes(n->left, buf, depth + 1, codes))
		return (0);
	buf[depth] = '1';
	return (build_codes(n->right, buf, depth + 1, codes));
}

/**
 * free_codes - free every code string
 * @codes: table of ALPHABET strings (entries may be NULL)
 */
void free_codes(char *codes[])
{
	int i;

	for (i = 0; i < ALPHABET; i++)
	{
		free(codes[i]);
		codes[i] = NULL;
	}
}

/**
 * describe_char - make a readable label for a byte
 * @c: byte value
 * @out: destination buffer
 * @size: size of @out
 */
void describe_char(int c, char *out, size_t size)
{
	switch (c)
	{
	case '\n':
		snprintf(out, size, "'\\n'");
		return;
	case '\r':
		snprintf(out, size, "'\\r'");
		return;
	case '\t':
		snprintf(out, size, "'\\t'");
		return;
	case ' ':
		snprintf(out, size, "' '");
		return;
	}
	if (isprint(c))
		snprintf(out, size, "'%c'", c);
	else
		snprintf(out, size, "0x%02X", c);
}

/**
 * print_frequencies - show the frequency table
 * @freq: frequency of each byte value
 */
void print_frequencies(const size_t freq[])
{
	char label[16];
	int s;

	puts("\nCharacter frequencies:");
	puts("  Char     Frequency");
	for (s = 0; s < ALPHABET; s++)
	{
		if (freq[s] == 0)
			continue;
		describe_char(s, label, sizeof(label));
		printf("  %-8s %lu\n", label, (unsigned long)freq[s]);
	}
}

/**
 * print_codes - show the Huffman code of every symbol
 * @codes: table of code strings
 */
void print_codes(char *codes[])
{
	char label[16];
	int s;

	puts("\nHuffman codes:");
	puts("  Char     Code");
	for (s = 0; s < ALPHABET; s++)
	{
		if (codes[s] == NULL)
			continue;
		describe_char(s, label, sizeof(label));
		printf("  %-8s %s\n", label, codes[s]);
	}
}
