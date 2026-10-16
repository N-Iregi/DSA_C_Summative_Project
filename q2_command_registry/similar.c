#include "registry.h"

/**
 * edit_distance - Levenshtein distance, ignoring letter case
 * @a: first string (at most MAX_CMD_LEN characters)
 * @b: second string (at most MAX_CMD_LEN characters)
 *
 * Return: fewest single-character insertions, deletions or
 * substitutions that turn @a into @b
 */
int edit_distance(const char *a, const char *b) {
  size_t la = strlen(a), lb = strlen(b), i, j;
  int prev[MAX_CMD_LEN + 2], cur[MAX_CMD_LEN + 2], cost, best;

  for (j = 0; j <= lb; j++)
    prev[j] = (int)j;
  for (i = 1; i <= la; i++) {
    cur[0] = (int)i;
    for (j = 1; j <= lb; j++) {
      cost =
          tolower((unsigned char)a[i - 1]) != tolower((unsigned char)b[j - 1]);
      best = prev[j - 1] + cost;
      if (prev[j] + 1 < best)
        best = prev[j] + 1;
      if (cur[j - 1] + 1 < best)
        best = cur[j - 1] + 1;
      cur[j] = best;
    }
    memcpy(prev, cur, (lb + 1) * sizeof(int));
  }
  return (prev[lb]);
}

/**
 * find_closest - find the stored command nearest to the input
 * @node: root of the subtree to search
 * @input: text typed by the technician
 * @best: receives the closest node found so far
 * @best_dist: receives that node's edit distance
 *
 * The walk is in-order, so on a tie the alphabetically first wins.
 */
void find_closest(const node_t *node, const char *input, const node_t **best,
                  int *best_dist) {
  int d;

  if (node == NULL)
    return;
  find_closest(node->left, input, best, best_dist);
  d = edit_distance(input, node->command);
  if (d < *best_dist) {
    *best_dist = d;
    *best = node;
  }
  find_closest(node->right, input, best, best_dist);
}
