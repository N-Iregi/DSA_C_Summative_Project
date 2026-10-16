# DSA in C: Summative Project

Five command-line programs written in C, each built around a different data structure or algorithm:
a doubly linked list, a binary search tree, a directed graph, Dijkstra's shortest-path algorithm,
and Huffman coding.

**Author:** Neville Iregi, African Leadership University
**Course:** Data Structures and Algorithms in C

| # | Program | Data structure / algorithm | Folder |
|---|---------|----------------------------|--------|
| 1 | Embedded Device Event Logger | Doubly linked list (25-event cap) | `q1_event_logger/` |
| 2 | Network Device Command Registry | Binary search tree + edit-distance suggestions | `q2_command_registry/` |
| 3 | Server Dependency Network Analyzer | Directed graph (adjacency lists) + adjacency matrix | `q3_dependency_graph/` |
| 4 | Emergency Medical Supply Routing | Weighted graph + Dijkstra's algorithm | `q4_medical_routing/` |
| 5 | Firmware Log Compression Utility | Min-heap + Huffman tree, lossless compression | `q5_huffman/` |

## Requirements

- Linux, macOS or WSL
- `gcc` (any recent version)

No Makefile is used. Each program is compiled with one `gcc` command.

## Build and run

Open a terminal in the program's folder. The flags are strict on purpose: `-Werror` turns every
warning into an error, so the code only compiles if it is clean.

| Program | Compile (inside its folder) | Run |
|---------|-----------------------------|-----|
| Q1 | `gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o event_logger` | `./event_logger` |
| Q2 | `gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o command_registry` | `./command_registry` or `./command_registry myfile.txt` |
| Q3 | `gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o dependency_graph` | `./dependency_graph` |
| Q4 | `gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o medical_routing` | `./medical_routing` |
| Q5 | `gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o huffman` | `./huffman` (needs `device_log.txt` in the same folder) |

Example:

```
cd q1_event_logger
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o event_logger
./event_logger
```

### Checking for memory errors

Build any program with AddressSanitizer and UndefinedBehaviorSanitizer. It prints nothing when
memory is handled correctly, and prints a detailed report on a leak, overflow or use-after-free.

```
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 -g -fsanitize=address,undefined *.c -o program_asan
./program_asan
```

## Repository layout

Each question is a small project: one header (`.h`) with the data structures and every function
prototype, and several short `.c` files with one job each.

```
.
├── q1_event_logger/        event_logger.h, main.c, input.c, logger.c, navigate.c,
│                           display.c, add_event.c, session.c
├── q2_command_registry/    registry.h, main.c, input.c, tree.c, similar.c, load.c,
│                           audit.c, verify.c, menu.c, commands.txt
├── q3_dependency_graph/    graph.h, main.c, input.c, vertex.c, edge.c, init.c,
│                           query.c, matrix.c, commands.c
├── q4_medical_routing/     routing.h, main.c, input.c, network.c, dijkstra.c,
│                           display.c, route_cmd.c, road_cmd.c
└── q5_huffman/             huffman.h, main.c, input.c, heap.c, tree.c, codes.c,
                            bitio.c, treeio.c, files.c, compress.c, decompress.c,
                            verify.c, device_log.txt
```

In every folder `main.c` holds the command loop, `input.c` holds safe keyboard input, and the
remaining files hold the data structure and one command each.

---

## Question 1: Embedded Device Event Logger

An industrial device records events (temperature warnings, low battery, sensor failure,
communication errors). With little memory, it keeps only the **25 most recent events** in a
dynamically allocated doubly linked list.

- **Structure:** each `event_t` node has an id, type, device id, timestamp, severity and
  `prev`/`next` pointers. `logger_t` holds `head` (oldest), `tail` (newest), `current` (the
  event being viewed), `count` and `next_id`.
- **Commands:** `n` next (newer), `p` previous (older), `a` add, `c` show current, `x` clear all,
  `q` save and quit. Extras: `l` list all, `h` help.
- **The 25-event limit:** when a 26th event arrives, the oldest node is removed and freed. If
  `current` pointed at it, `current` moves to the next event, so it never dangles.
- **Output file:** `q` writes the session to `event_session.log`.
- **Handled cases:** empty list, first insert, both ends of the list, eviction when `current` is
  the oldest event, `malloc` failure (the new node is allocated first, so nothing is lost),
  invalid input, over-long lines and end-of-input.

## Question 2: Network Device Command Registry

Approved device commands are loaded from a text file into a **binary search tree** (maximum 50
commands). A technician can verify a command; only an exact match is approved.

- **Commands:** `v` verify, `p` print all (in-order traversal gives alphabetical order),
  `s` count, `x` clear, `q` save and quit.
- **Verification:** an exact match prints `COMMAND APPROVED`. A near miss prints a suggestion such
  as `Did you mean: SHOW_STATUS?` (found with Levenshtein edit distance) and is **not** approved.
  Anything else prints `UNKNOWN COMMAND`.
- **Audit log:** every attempt that is not an exact match is appended to `audit.log`, for example
  `2026-10-14 07:57:01 | UNKNOWN | RESET_INTERFACES`. Approved commands are never logged.
- **Input file:** `commands.txt` by default, or the path given as the first argument. Empty
  files, duplicate lines, blank lines, Windows line endings and over-long lines are handled, and
  loading stops at 50 commands.
- **Output files:** `audit.log` and `session_summary.txt`.
- **Known weakness:** the tree is not self-balancing. A sorted input file turns it into a linked
  list (depth 49 for 50 commands).

## Question 3: Server Dependency Network Analyzer

Servers are vertices and "A depends on B" is a directed edge A -> B, stored as adjacency lists.
The program starts with six servers and seven dependencies.

- **Commands:** `a` add server, `r` remove server, `c` create dependency, `d` delete dependency,
  `q` query a server, `m` show the adjacency matrix, `x` exit. Extra: `h` help.
- **Queries:** shows what a server depends on and what depends on it.
- **Removing a server** also removes every edge that points to it or leaves it, and reports how
  many links were removed.
- **Adjacency matrix:** built from the live lists each time it is displayed, so it is always
  current. `matrix[A][B] = 1` when A depends on B.
- **Handled cases:** unknown servers, duplicate servers (case-insensitive), duplicate
  dependencies, self-dependencies, dependencies with a missing server, and every operation on an
  empty graph.
- **Known limits:** only direct dependencies are reported, and cycles are not detected.

## Question 4: Emergency Medical Supply Routing

A weighted, undirected road network (six locations, nine roads) stored as an adjacency matrix.
**Dijkstra's algorithm** finds the shortest route and a predecessor array rebuilds the route.

- **Menu:** `1` display locations, `2` find shortest route, `3` display last route distance,
  `4` exit. Extras: `5` add or update a road, `6` add a location (these make the invalid-distance
  and unreachable-destination cases demonstrable).
- **Example:** Central Warehouse to District Hospital gives the direct road, **14 km**. Central
  Warehouse to Health Center C goes through Distribution Center and Health Center A (**17 km**),
  which is shorter than the two-road route through the hospital (20 km).
- **Handled cases:** unknown location, same start and destination, unreachable destination, and
  zero, negative, non-numeric or huge distances.
- **Memory:** no dynamic allocation; everything is fixed-size (maximum 16 locations).

## Question 5: Embedded Firmware Log Compression Utility

Compresses `device_log.txt` with **Huffman coding**, restores it, and checks that nothing was
lost.

- **Menu:** `1` compress, `2` decompress (then verifies automatically), `3` verify, `4` exit.
- **Files:** `device_log.txt` -> `device_log.huff` -> `device_log_restored.txt`.
- **Compressed file format:** 2 magic bytes (`HF`), the original size in 4 bytes, then a bit
  stream holding the Huffman tree in pre-order followed by the encoded data. Storing the tree
  (not the frequency table) keeps the overhead small, and storing the size lets the decoder
  ignore padding bits and detect truncation.
- **Sample result:** 482 bytes compress to 327 bytes (ratio 67.84%, 32.16% saved), and
  verification reports `SUCCESS`.
- **Handled cases:** missing files, empty files, single-character files, binary data (all 256
  byte values), corrupt or truncated archives (an incomplete output file is deleted), and invalid
  menu input.
- **Honest limits:** tiny files and random data grow because of the header and tree, and there
  is no checksum inside the compressed data.

---

## Testing

- **Compiler:** all programs compile with `-Wall -Werror -Wextra -pedantic -std=gnu89`.
- **Sanitizers:** every scenario below was also run under AddressSanitizer and
  UndefinedBehaviorSanitizer with no leaks, overflows or undefined behaviour reported.
- **Q1:** 27 events inserted; 25 kept (ids 3 to 27); the cursor was moved correctly when its
  node was evicted.
- **Q2:** exact, near-miss and unknown commands; empty, duplicate, CRLF, missing and 60-line
  files; `audit.log` contained only the non-approved attempts.
- **Q3:** the matrix matched the specified edges; adding and removing servers kept every row
  consistent; the empty graph was handled.
- **Q4:** all 30 ordered location pairs matched an independent Floyd-Warshall computation, and
  every printed route was re-added from the road table.
- **Q5:** nine round trips (empty file, 1 byte, single repeated character, binary data, all 256
  byte values, 200 KB of random data, an 840 KB log, a deep-tree case) restored byte for byte;
  six kinds of corrupt archive were rejected without a crash.

## Code style

The code follows the **Betty** style used at ALU/Holberton and passes the Betty style and
documentation checkers with no findings: tabs, braces on their own lines, `return (value);`,
lines of at most 80 columns, at most 5 functions per file, at most 40 lines per function, and a
documentation comment on every function.

## Generated files

Running the programs creates these files, which do not belong in version control:
`event_session.log`, `audit.log`, `session_summary.txt`, `device_log.huff`,
`device_log_restored.txt` and the compiled binaries.

## Limitations (summary)

- Q1: the saved session is not reloaded; the capacity is a compile-time constant.
- Q2: the tree is not balanced; matching is case-sensitive.
- Q3: no cycle detection; the graph is not saved to disk.
- Q4: O(V²) Dijkstra suits a small map; whole kilometres and two-way roads only.
- Q5: no integrity check inside the data; the whole file is held in memory.
