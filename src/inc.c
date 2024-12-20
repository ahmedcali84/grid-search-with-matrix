
#include "queue.h"
#include "grid.h"

#define UNUSED(x) ((void) x)

Grid *grid_m;

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s file_path StartAndEndJoined.\n", argv[0]);
        return 1;
    }
    
    // NOTE: Allocate Memory For Grid

    grid_m = grid_alloc(grid_m);
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

    Matrix A = adjacency_matrix(G);
    // NOTE: The Adjacency data is too big to be printed(takes forever) on large files
    PRINT(A);
    SHAPE(A);

    char Start = argv[2][0];
    char End = argv[2][1];
    printf("Start: %c is %d\nEnd: %c is %d\n", argv[2][0], argv[2][0], argv[2][1], argv[2][1]);
    UNUSED(Start);
    UNUSED(End);

    // NOTE: Deallocate Allocated memory
    UNLOAD(&G);
    UNLOAD(&A);
    // queue_dealloc(&queue);
    grid_dealloc(grid_m);
    return 0;
}
