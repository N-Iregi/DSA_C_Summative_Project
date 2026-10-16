#include "huffman.h"

/**
 * count_frequencies - count how often each byte value occurs
 * @data: file contents
 * @size: number of bytes in @data
 * @freq: receives one counter per byte value (must start at zero)
 *
 * Return: number of distinct byte values
 */
static int count_frequencies(const unsigned char *data, size_t size,
		size_t freq[])
{
	size_t i;
	int s, distinct = 0;

	for (i = 0; i < size; i++)
		freq[data[i]]++;
	for (s = 0; s < ALPHABET; s++)
	{
		if (freq[s] != 0)
			distinct++;
	}
	return (distinct);
}

/**
 * make_codes - build the tree and the code table, and show them
 * @freq: byte frequencies
 * @root: receives the tree root
 * @codes: receives the code table
 *
 * Return: 1 on success, 0 if malloc failed
 */
static int make_codes(const size_t freq[], huff_node_t **root,
		char *codes[])
{
	char buf[ALPHABET + 2];

	*root = build_tree(freq);
	if (*root == NULL || !build_codes(*root, buf, 0, codes))
	{
		puts("Error: out of memory while building the Huffman tree.");
		return (0);
	}
	print_codes(codes);
	return (1);
}

/**
 * write_archive - write header, tree and encoded data to the .huff file
 * @root: Huffman tree (NULL for an empty file)
 * @codes: code table
 * @data: original file contents
 * @size: number of bytes in @data
 *
 * Return: 1 on success, 0 on failure
 */
static int write_archive(const huff_node_t *root, char *codes[],
		const unsigned char *data, size_t size)
{
	FILE *out;
	bit_writer_t w;
	size_t i;
	const char *c;
	int shift;

	out = fopen(COMPRESSED_FILE, "wb");
	if (out == NULL)
	{
		printf("Error: cannot create '%s'.\n", COMPRESSED_FILE);
		return (0);
	}
	w.fp = out;
	w.acc = 0;
	w.nbits = 0;
	w.error = 0;
	fputc('H', out);
	fputc('F', out);
	for (shift = 24; shift >= 0; shift -= 8)
		fputc((int)((size >> shift) & 0xFF), out);
	if (root != NULL)
	{
		write_tree(&w, root);
		for (i = 0; i < size; i++)
		{
			for (c = codes[data[i]]; *c != '\0'; c++)
				bw_put(&w, *c - '0');
		}
	}
	bw_flush(&w);
	return (fclose(out) == 0 && !w.error);
}

/**
 * report_sizes - print original size, compressed size and the ratio
 * @size: original size in bytes
 */
static void report_sizes(size_t size)
{
	long csize = file_size(COMPRESSED_FILE);
	double ratio;

	printf("\nCompressed log saved to %s\n", COMPRESSED_FILE);
	printf("Original size:     %lu bytes\n", (unsigned long)size);
	printf("Compressed size:   %ld bytes (header and tree included)\n",
			csize);
	if (size == 0 || csize < 0)
		return;
	ratio = 100.0 * (double)csize / (double)size;
	printf("Compression ratio: %.2f%% (compressed / original)\n", ratio);
	printf("Space saved:       %.2f%%\n", 100.0 - ratio);
	if ((size_t)csize >= size)
		puts("Note: too small to beat the header overhead.");
}

/**
 * do_compress - compress device_log.txt into device_log.huff
 */
void do_compress(void)
{
	size_t size = 0, freq[ALPHABET] = {0};
	unsigned char *data;
	huff_node_t *root = NULL;
	char *codes[ALPHABET] = {NULL};
	int distinct, ok = 1;

	data = read_whole_file(INPUT_FILE, &size);
	if (data == NULL)
		return;
	distinct = count_frequencies(data, size, freq);
	printf("\nLoaded %s: %lu bytes, %d distinct character(s).\n",
			INPUT_FILE, (unsigned long)size, distinct);
	if (size > 0)
	{
		print_frequencies(freq);
		ok = make_codes(freq, &root, codes);
	}
	else
		puts("The log is empty; only the header will be written.");
	if (ok && write_archive(root, codes, data, size))
		report_sizes(size);
	else if (ok)
	{
		puts("Error: failed while writing the compressed file.");
		remove(COMPRESSED_FILE);
	}
	free_tree(root);
	free_codes(codes);
	free(data);
}
