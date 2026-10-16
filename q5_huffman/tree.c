#include "huffman.h"

/**
 * is_leaf - check whether a node holds a symbol
 * @n: node to test
 *
 * Return: 1 for a leaf, 0 for an internal node
 */
int is_leaf(const huff_node_t *n)
{
	return (n->left == NULL && n->right == NULL);
}

/**
 * free_tree - free a whole tree, children before parent
 * @n: root of the (sub)tree, may be NULL
 */
void free_tree(huff_node_t *n)
{
	if (n == NULL)
		return;
	free_tree(n->left);
	free_tree(n->right);
	free(n);
}

/**
 * load_leaves - create one leaf per used byte and push it on the heap
 * @freq: frequency of each byte value
 * @heap: heap to fill
 * @order: running creation counter
 *
 * Return: 1 on success, 0 if malloc failed
 */
static int load_leaves(const size_t freq[], min_heap_t *heap,
		unsigned int *order)
{
	huff_node_t *leaf;
	int s;

	for (s = 0; s < ALPHABET; s++)
	{
		if (freq[s] == 0)
			continue;
		leaf = calloc(1, sizeof(huff_node_t));
		if (leaf == NULL)
			return (0);
		leaf->symbol = (unsigned char)s;
		leaf->freq = freq[s];
		leaf->order = (*order)++;
		heap_push(heap, leaf);
	}
	return (1);
}

/**
 * merge_nodes - join the two rarest nodes under a new parent
 * @heap: heap holding at least two nodes
 * @order: running creation counter
 *
 * Return: 1 on success, 0 if malloc failed (the two nodes are freed)
 */
static int merge_nodes(min_heap_t *heap, unsigned int *order)
{
	huff_node_t *a, *b, *parent;

	a = heap_pop(heap);
	b = heap_pop(heap);
	parent = calloc(1, sizeof(huff_node_t));
	if (parent == NULL)
	{
		free_tree(a);
		free_tree(b);
		return (0);
	}
	parent->freq = a->freq + b->freq;
	parent->order = (*order)++;
	parent->left = a;
	parent->right = b;
	heap_push(heap, parent);
	return (1);
}

/**
 * build_tree - build the Huffman tree from byte frequencies
 * @freq: frequency of each byte value
 *
 * Return: the root, or NULL if no byte is used or malloc failed
 */
huff_node_t *build_tree(const size_t freq[])
{
	min_heap_t heap;
	unsigned int order = 0;
	huff_node_t *root = NULL;
	int ok;

	heap.size = 0;
	ok = load_leaves(freq, &heap, &order);
	while (ok && heap.size > 1)
		ok = merge_nodes(&heap, &order);
	if (ok && heap.size == 1)
		root = heap_pop(&heap);
	while (heap.size > 0)
		free_tree(heap_pop(&heap));
	return (root);
}
