#include "registry.h"

/**
 * tree_search - look for an exact command, walking down the tree
 * @root: root of the tree
 * @cmd: command to find
 * @depth: receives the depth of the last node visited
 * @comparisons: receives the number of strcmp calls made
 *
 * Return: the matching node, or NULL if @cmd is not stored
 */
node_t *tree_search(const node_t *root, const char *cmd, int *depth,
                    int *comparisons) {
  const node_t *cur = root;
  int cmp;

  *depth = 0;
  *comparisons = 0;
  while (cur != NULL) {
    cmp = strcmp(cmd, cur->command);
    (*comparisons)++;
    if (cmp == 0)
      return ((node_t *)cur);
    cur = (cmp < 0) ? cur->left : cur->right;
    (*depth)++;
  }
  return (NULL);
}

/**
 * registry_insert - add a command to the tree
 * @reg: registry to add to
 * @cmd: command text (at most MAX_CMD_LEN characters)
 *
 * Return: INS_OK, INS_DUPLICATE, INS_FULL or INS_NOMEM
 */
insert_result_t registry_insert(registry_t *reg, const char *cmd) {
  node_t *parent = NULL, *cur = reg->root, *node;
  int cmp = 0;

  while (cur != NULL) {
    cmp = strcmp(cmd, cur->command);
    if (cmp == 0)
      return (INS_DUPLICATE);
    parent = cur;
    cur = (cmp < 0) ? cur->left : cur->right;
  }
  if (reg->count >= MAX_COMMANDS)
    return (INS_FULL);
  node = calloc(1, sizeof(node_t));
  if (node == NULL)
    return (INS_NOMEM);
  snprintf(node->command, sizeof(node->command), "%s", cmd);
  if (parent == NULL)
    reg->root = node;
  else if (cmp < 0)
    parent->left = node;
  else
    parent->right = node;
  reg->count++;
  return (INS_OK);
}

/**
 * tree_free - free a subtree, children before parent (post-order)
 * @node: root of the subtree
 */
void tree_free(node_t *node) {
  if (node == NULL)
    return;
  tree_free(node->left);
  tree_free(node->right);
  free(node);
}

/**
 * registry_clear - free every node and empty the registry
 * @reg: registry to clear
 *
 * Return: number of commands that were removed
 */
int registry_clear(registry_t *reg) {
  int removed = reg->count;

  tree_free(reg->root);
  reg->root = NULL;
  reg->count = 0;
  return (removed);
}

/**
 * tree_print_inorder - print commands in alphabetical order
 * @node: root of the subtree
 * @index: running counter used for numbering
 */
void tree_print_inorder(const node_t *node, int *index) {
  if (node == NULL)
    return;
  tree_print_inorder(node->left, index);
  printf("  %2d. %s\n", ++(*index), node->command);
  tree_print_inorder(node->right, index);
}
