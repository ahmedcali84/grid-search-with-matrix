#include "queue.h"
#include "grid.h"
#include "search.h"

#define UNUSED(x) ((void) x)

void print_mark(Matrix *G, int *path, int index) {
    for (size_t i = 0; i < G->nrows; ++i) {
        for (size_t j = 0; j < G->ncols; ++j) {
            int id = i * G->ncols + j;  // Convert 2D (i, j) to 1D index

            bool is_path = false;
            // Check if current (i, j) is in the path
            for (int k = 0; k < index; ++k) {
                if (path[k] == id) {
                    is_path = true;
                    break;
                }
            }

            // If it is in the path, print '*', otherwise print the grid element
            if (is_path) {
                printf("* ");
            } else {
                char c;
                GET_ELEMENT(*G, i, j, &c);  // Get the element at (i, j)
                printf("%c ", c);  // Print the element
            }
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s file_path.\n", argv[0]);
        return 1;
    }

    // NOTE: Allocate Memory For Grid
    Grid *grid_m = grid_alloc();
    if (grid_m == NULL) {
        fprintf(stderr, ALLOCATION_FAILED" For Grid.\n");
        return 1;
    }

    // NOTE: Read File
    const char *file_path_var = argv[1];
    if(!read_file(file_path_var, grid_m)) return 1;

    // NOTE: Load Grid Into Matrix
    Matrix G = load_grid_into_matrix(grid_m);
    PRINT(G);
    SHAPE(G);

    Matrix A = adjacency_matrix(G);
    // NOTE: The Adjacency data is too big to be printed(takes forever) on large files
    // PRINT(A);
    SHAPE(A);

    int path_to[A.ncols];
    size_t end_node = (G.nrows - 1) * G.ncols + (G.ncols - 1);
    int index;
    if(bfs(&A, 0, end_node, path_to , &index)) {
        for (int i = 0; i < index; ++i) {
            printf("%d ", path_to[i]);
        }
        printf("\n\n");
    } else {
        printf("Path Not Found.\n");
    }

    print_mark(&G , path_to , index);

    // NOTE: Deallocate Allocated memory
    UNLOAD(&G);
    UNLOAD(&A);
    grid_dealloc(grid_m);
    return 0;
}
