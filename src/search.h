#ifndef SEARCH_IMPLEMENTATION
#define SEARCH_IMPLEMENTATION

#include <matrix.h>
#include "queue.h"
#include "grid.h"

// integration of BFS
bool bfs(Matrix *adjacency, int start, int end, int *path_to_destination, void *index);

#endif
