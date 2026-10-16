#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "device_log.txt"
#define COMPRESSED_FILE "device_log.huff"
#define RESTORED_FILE "device_log_restored.txt"
#define ALPHABET 256
#define LINE_LEN 64

/**
 * struct huff_node_s - a node of the Huffman tree
 * @symbol: the byte stored in a leaf
 * @freq: how many times the symbol (or all leaves below) occurs
 * @order: creation order, used to break ties deterministically
 * @left: child reached by bit 0 (NULL in a leaf)
 * @right: child reached by bit 1 (NULL in a leaf)
 */
typedef struct huff_node_s
{
	unsigned char symbol;
	size_t freq;
	unsigned int order;
	struct huff_node_s *left;
	struct huff_node_s *right;
} huff_node_t;

/**
 * struct min_heap_s - priority queue of tree nodes
 * @items: heap array (smallest frequency at index 0)
 * @size: number of nodes in the heap
 */
typedef struct min_heap_s
{
	huff_node_t *items[ALPHABET];
	int size;
} min_heap_t;

/**
 * struct bit_writer_s - packs bits into bytes for a file
 * @fp: output file
 * @acc: bits collected so far
 * @nbits: how many bits are in @acc (0 to 7)
 * @error: set to 1 if a write failed
 */
typedef struct bit_writer_s
{
	FILE *fp;
	unsigned int acc;
	int nbits;
	int error;
} bit_writer_t;

/**
 * struct bit_reader_s - unpacks bits from the bytes of a file
 * @fp: input file
 * @cur: byte currently being consumed
 * @left: bits of @cur not yet consumed
 */
typedef struct bit_reader_s
{
	FILE *fp;
	int cur;
	int left;
} bit_reader_t;

/* input.c */
int read_line(char *buf, size_t size);
char *trim(char *s);

/* heap.c */
void heap_push(min_heap_t *h, huff_node_t *n);
huff_node_t *heap_pop(min_heap_t *h);

/* tree.c */
int is_leaf(const huff_node_t *n);
void free_tree(huff_node_t *n);
huff_node_t *build_tree(const size_t freq[]);

/* codes.c */
int build_codes(const huff_node_t *n, char *buf, int depth,
		char *codes[]);
void free_codes(char *codes[]);
void describe_char(int c, char *out, size_t size);
void print_frequencies(const size_t freq[]);
void print_codes(char *codes[]);

/* bitio.c */
void bw_put(bit_writer_t *w, int bit);
void bw_flush(bit_writer_t *w);
int br_get(bit_reader_t *r);

/* treeio.c */
void write_tree(bit_writer_t *w, const huff_node_t *n);
huff_node_t *read_leaf(bit_reader_t *r);
huff_node_t *read_tree(bit_reader_t *r, int depth);

/* files.c */
unsigned char *read_whole_file(const char *path, size_t *size_out);
long file_size(const char *path);

/* compress.c */
void do_compress(void);

/* decompress.c */
void do_decompress(void);

/* verify.c */
int files_equal(FILE *a, FILE *b);
void do_verify(void);

/* main.c */
void print_menu(void);
void run_choice(long choice);

#endif /* HUFFMAN_H */
