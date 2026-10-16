#include "huffman.h"

/**
 * read_header - check the magic bytes and read the original size
 * @in: open .huff file
 * @orig: receives the original size in bytes
 *
 * Return: 1 if the header is valid, 0 otherwise (after a message)
 */
static int read_header(FILE *in, uint32_t *orig)
{
	int m0, m1, c, i;

	m0 = fgetc(in);
	m1 = fgetc(in);
	if (m0 != 'H' || m1 != 'F')
	{
		puts("Error: not a valid .huff file (bad header).");
		return (0);
	}
	*orig = 0;
	for (i = 0; i < 4; i++)
	{
		c = fgetc(in);
		if (c == EOF)
		{
			puts("Error: compressed file is truncated (header).");
			return (0);
		}
		*orig = (*orig << 8) | (uint32_t)c;
	}
	return (1);
}

/**
 * decode_symbols - walk the tree bit by bit to recover the bytes
 * @r: bit reader positioned at the encoded data
 * @root: Huffman tree
 * @orig: number of bytes to produce
 * @out: restored file
 *
 * Return: 1 on success, 0 if the data ends early or a write fails
 */
static int decode_symbols(bit_reader_t *r, const huff_node_t *root,
		uint32_t orig, FILE *out)
{
	const huff_node_t *n;
	uint32_t i;
	int bit;

	for (i = 0; i < orig; i++)
	{
		n = root;
		if (is_leaf(n) && br_get(r) < 0)
			return (0);
		while (!is_leaf(n))
		{
			bit = br_get(r);
			if (bit < 0)
				return (0);
			n = bit ? n->right : n->left;
		}
		if (fputc(n->symbol, out) == EOF)
			return (0);
	}
	return (1);
}

/**
 * load_tree - read the header and rebuild the Huffman tree
 * @in: open .huff file
 * @r: bit reader to set up on @in
 * @orig: receives the original size
 * @root: receives the tree (NULL for an empty original)
 *
 * Return: 1 on success, 0 on a bad header or corrupt tree
 */
static int load_tree(FILE *in, bit_reader_t *r, uint32_t *orig,
		huff_node_t **root)
{
	*root = NULL;
	if (!read_header(in, orig))
		return (0);
	r->fp = in;
	r->cur = 0;
	r->left = 0;
	if (*orig == 0)
		return (1);
	*root = read_tree(r, 0);
	if (*root == NULL)
	{
		puts("Error: corrupt file (cannot rebuild the Huffman tree).");
		return (0);
	}
	return (1);
}

/**
 * do_decompress - restore device_log.txt from device_log.huff
 *
 * Verification runs automatically after a successful restore.
 */
void do_decompress(void)
{
	FILE *in, *out;
	bit_reader_t r;
	huff_node_t *root;
	uint32_t orig;
	int ok;

	in = fopen(COMPRESSED_FILE, "rb");
	if (in == NULL)
	{
		printf("Error: cannot open '%s' (%s).\n", COMPRESSED_FILE,
				strerror(errno));
		return;
	}
	if (!load_tree(in, &r, &orig, &root))
	{
		fclose(in);
		return;
	}
	out = fopen(RESTORED_FILE, "wb");
	if (out == NULL)
	{
		printf("Error: cannot create '%s'.\n", RESTORED_FILE);
		free_tree(root);
		fclose(in);
		return;
	}
	ok = decode_symbols(&r, root, orig, out);
	ok = (fclose(out) == 0) && ok;
	fclose(in);
	free_tree(root);
	if (!ok)
	{
		puts("Error: data truncated/corrupt or output not writable.");
		remove(RESTORED_FILE);
		return;
	}
	printf("Decompressed %lu byte(s) to %s\n\n", (unsigned long)orig,
			RESTORED_FILE);
	do_verify();
}
