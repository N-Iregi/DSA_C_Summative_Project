#ifndef REGISTRY_H
#define REGISTRY_H

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMMANDS 50
#define MAX_CMD_LEN 63
#define LINE_LEN 256
#define DEFAULT_FILE "commands.txt"
#define AUDIT_LOG "audit.log"
#define SUMMARY_FILE "session_summary.txt"

/**
 * struct node_s - one approved command (a BST node)
 * @command: the command text
 * @left: subtree of commands that sort before this one
 * @right: subtree of commands that sort after this one
 */
typedef struct node_s
{
	char command[MAX_CMD_LEN + 1];
	struct node_s *left;
	struct node_s *right;
} node_t;

/**
 * struct registry_s - the command registry
 * @root: root of the binary search tree
 * @count: number of commands stored (never above MAX_COMMANDS)
 */
typedef struct registry_s
{
	node_t *root;
	int count;
} registry_t;

/**
 * struct stats_s - results of this session's verifications
 * @approved: exact matches
 * @suggested: not approved, but a similar command was suggested
 * @unknown: not approved and nothing similar found
 */
typedef struct stats_s
{
	int approved;
	int suggested;
	int unknown;
} stats_t;

/**
 * enum insert_result_e - outcome of inserting a command
 * @INS_OK: inserted
 * @INS_DUPLICATE: command already present
 * @INS_FULL: registry already holds MAX_COMMANDS
 * @INS_NOMEM: malloc failed
 */
typedef enum insert_result_e
{
	INS_OK,
	INS_DUPLICATE,
	INS_FULL,
	INS_NOMEM
} insert_result_t;

/**
 * struct load_stats_s - counts of lines that were not loaded
 * @duplicates: commands already in the tree
 * @too_long: lines longer than MAX_CMD_LEN
 * @over_limit: valid commands beyond MAX_COMMANDS
 * @bad_chars: lines with non-printable characters
 * @nomem: lines lost to a failed malloc
 */
typedef struct load_stats_s
{
	int duplicates;
	int too_long;
	int over_limit;
	int bad_chars;
	int nomem;
} load_stats_t;

/* input.c */
int read_line(char *buf, size_t size);
char *trim(char *s);
int is_printable_text(const char *s);

/* tree.c */
node_t *tree_search(const node_t *root, const char *cmd, int *depth,
		int *comparisons);
insert_result_t registry_insert(registry_t *reg, const char *cmd);
void tree_free(node_t *node);
int registry_clear(registry_t *reg);
void tree_print_inorder(const node_t *node, int *index);

/* similar.c */
int edit_distance(const char *a, const char *b);
void find_closest(const node_t *node, const char *input,
		const node_t **best, int *best_dist);

/* load.c */
int load_commands(registry_t *reg, const char *path);

/* audit.c */
void timestamp_now(char *out, size_t size);
void audit_log_unknown(const char *cmd);
void save_session(const registry_t *reg, const stats_t *stats);

/* verify.c */
void verify_command(const registry_t *reg, const char *input,
		stats_t *stats);
void cmd_verify(const registry_t *reg, stats_t *stats);

/* menu.c */
void print_help(void);
void cmd_print(const registry_t *reg);
void cmd_count(const registry_t *reg);
void cmd_clear(registry_t *reg);

/* main.c */
void run_command(registry_t *reg, stats_t *stats, char cmd);

#endif /* REGISTRY_H */
