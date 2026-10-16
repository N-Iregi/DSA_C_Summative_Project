#include "huffman.h"

/**
 * node_less - order two nodes: lower frequency first, then older first
 * @a: first node
 * @b: second node
 *
 * Return: 1 if @a must come out of the heap before @b, else 0
 */
static int node_less(const huff_node_t *a, const huff_node_t *b)
{
	return (a->freq < b->freq ||
			(a->freq == b->freq && a->order < b->order));
}

/**
 * heap_push - insert a node and sift it up
 * @h: heap (must have room for ALPHABET nodes)
 * @n: node to insert
 */
void heap_push(min_heap_t *h, huff_node_t *n)
{
	int i, parent;
	huff_node_t *tmp;

	i = h->size++;
	h->items[i] = n;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!node_less(h->items[i], h->items[parent]))
			break;
		tmp = h->items[i];
		h->items[i] = h->items[parent];
		h->items[parent] = tmp;
		i = parent;
	}
}

/**
 * heap_pop - remove the smallest node and sift the last one down
 * @h: non-empty heap
 *
 * Return: the node with the lowest frequency
 */
huff_node_t *heap_pop(min_heap_t *h)
{
	huff_node_t *top = h->items[0], *tmp;
	int i = 0, l, r, m;

	h->items[0] = h->items[--h->size];
	while (1)
	{
		l = 2 * i + 1;
		r = l + 1;
		m = i;
		if (l < h->size && node_less(h->items[l], h->items[m]))
			m = l;
		if (r < h->size && node_less(h->items[r], h->items[m]))
			m = r;
		if (m == i)
			break;
		tmp = h->items[i];
		h->items[i] = h->items[m];
		h->items[m] = tmp;
		i = m;
	}
	return (top);
}
