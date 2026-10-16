#include "registry.h"

/**
 * report_miss - handle a command that is not an exact match
 * @reg: registry to compare against
 * @input: text typed by the technician
 * @stats: verification counters
 *
 * Prints a "Did you mean" suggestion when a stored command is close
 * enough, otherwise UNKNOWN COMMAND. Neither case is approved.
 */
static void report_miss(const registry_t *reg, const char *input,
                        stats_t *stats) {
  const node_t *best = NULL;
  int best_dist = MAX_CMD_LEN + 1, limit;

  find_closest(reg->root, input, &best, &best_dist);
  limit = (strlen(input) <= 4) ? 1 : 2;
  if (best != NULL && best_dist <= limit) {
    printf("Did you mean: %s?\n", best->command);
    puts("  (not approved - only an exact match is accepted)");
    stats->suggested++;
    return;
  }
  puts("UNKNOWN COMMAND");
  if (reg->root == NULL)
    puts("  (the registry is empty)");
  stats->unknown++;
}

/**
 * verify_command - check one command against the registry
 * @reg: registry to search
 * @input: validated command text
 * @stats: verification counters
 */
void verify_command(const registry_t *reg, const char *input, stats_t *stats) {
  int depth, comparisons;

  if (tree_search(reg->root, input, &depth, &comparisons) != NULL) {
    puts("COMMAND APPROVED");
    printf("  (found at tree depth %d after %d comparison(s))\n", depth,
           comparisons);
    stats->approved++;
    return;
  }
  audit_log_unknown(input);
  report_miss(reg, input, stats);
}

/**
 * cmd_verify - ask for a command, validate the input and verify it
 * @reg: registry to search
 * @stats: verification counters
 */
void cmd_verify(const registry_t *reg, stats_t *stats) {
  char line[LINE_LEN], *input;
  int r;

  printf("Enter command to verify: ");
  r = read_line(line, sizeof(line));
  input = trim(line);
  if (r < 0 || *input == '\0') {
    puts("Invalid input: no command entered.");
    return;
  }
  if (r == 1 || strlen(input) > MAX_CMD_LEN) {
    printf("Invalid input: at most %d characters.\n", MAX_CMD_LEN);
    return;
  }
  if (!is_printable_text(input)) {
    puts("Invalid input: non-printable characters.");
    return;
  }
  verify_command(reg, input, stats);
}
