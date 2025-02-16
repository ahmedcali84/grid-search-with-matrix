#include "search.h"

bool bfs(Matrix *adjacency, int start, int end, int *path_to_destination, void *index) {
    Queue q;
    init_queue(&q);  // Initialize the queue
    enqueue(&q, start);  // Start with the start node
    bool visited[adjacency->nrows * adjacency->ncols];
    for (size_t i = 0; i < adjacency->nrows * adjacency->ncols; i++) {
        visited[i] = false;
    }
    visited[start] = true;

    // Track the parent of each node for path reconstruction
    int parent[adjacency->nrows*adjacency->ncols];
    for (size_t i = 0; i < adjacency->nrows * adjacency->ncols; i++) {
        parent[i] = -1;
    }

    while (q.head != NULL) {
        int current = dequeue(&q);

        // Check if we reached the end node
        if (current == end) {
            // Reconstruct the path using the parent array
            printf("Path Found.\n");
            int path_node = end;
            int path[adjacency->nrows];
            int path_index = 0;

            // Collect the path from end to start
            while (path_node != -1) {
                path[path_index++] = path_node;
                path_node = parent[path_node];
            }

            for (int i = path_index - 1; i >= 0; i--) {
                path_to_destination[path_index - i - 1] = path[i];
            }

            memcpy(index , &path_index , sizeof(int));
            
            queue_dealloc(&q);
            return true;  // Path found
        }

        // Explore neighbors
        for (size_t i = 0; i < adjacency->ncols; i++) {
            bool is_adjacent;
            GET_ELEMENT(*adjacency, current, i, &is_adjacent);

            if (is_adjacent && !visited[i]) {
                enqueue(&q, i);
                visited[i] = true;
                parent[i] = current;  // Track the parent node
            }
        }
    }
    queue_dealloc(&q);
    return false;  // Path not found
}
