#include "huffman.h"

/**
 * write_tree - store the tree in pre-order
 * @w: bit writer
 * @n: node to write
 *
 * An internal node is the bit 0. A leaf is the bit 1 followed by the
 * eight bits of its symbol.
 */
void write_tree(bit_writer_t *w, const huff_node_t *n)
{
	int b;

	if (is_leaf(n))
	{
		bw_put(w, 1);
		for (b = 7; b >= 0; b--)
			bw_put(w, (n->symbol >> b) & 1);
		return;
	}
	bw_put(w, 0);
	write_tree(w, n->left);
	write_tree(w, n->right);
}

/**
 * read_leaf - read the 8 symbol bits of a leaf
 * @r: bit reader
 *
 * Return: a new leaf, or NULL on truncated data or malloc failure
 */
huff_node_t *read_leaf(bit_reader_t *r)
{
	huff_node_t *n;
	int sym = 0, i, b;

	n = calloc(1, sizeof(huff_node_t));
	if (n == NULL)
		return (NULL);
	for (i = 0; i < 8; i++)
	{
		b = br_get(r);
		if (b < 0)
		{
			free(n);
			return (NULL);
		}
		sym = (sym << 1) | b;
	}
	n->symbol = (unsigned char)sym;
	return (n);
}

/**
 * read_tree - rebuild the tree written by write_tree()
 * @r: bit reader
 * @depth: current depth (guards against corrupt, endless input)
 *
 * Return: the root of the subtree, or NULL if the data is corrupt
 */
huff_node_t *read_tree(bit_reader_t *r, int depth)
{
	huff_node_t *n;
	int bit;

	if (depth > ALPHABET)
		return (NULL);
	bit = br_get(r);
	if (bit < 0)
		return (NULL);
	if (bit == 1)
		return (read_leaf(r));
	n = calloc(1, sizeof(huff_node_t));
	if (n == NULL)
		return (NULL);
	n->left = read_tree(r, depth + 1);
	if (n->left != NULL)
		n->right = read_tree(r, depth + 1);
	if (n->left == NULL || n->right == NULL)
	{
		free_tree(n);
		return (NULL);
	}
	return (n);
}
