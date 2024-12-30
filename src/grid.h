#ifndef GRID_IMPLEMENTATION
#define GRID_IMPLEMENTATION

#define MAX_LINE_COUNT 256
#define INIT_CAPACITY 256
#define NEW_LINE() printf("\n")
#define READ_FILE_FAILED "Failed To Read File %s\n"

#include "grid.h"
#include <matrix.h>

// Declare the dir for neighbors (left, right, up, down, diagonal)
extern int dir[8][2];

typedef struct {
    char *buf;
    size_t count;
    size_t capacity;
} Line;

typedef struct {
    Line **items;
    size_t count;
    size_t capacity;
} Grid;

Grid *grid_alloc(Grid *grid);
void append_to_grid(Grid *grid, Line *line);
void grid_dealloc(Grid *g);
bool read_file(const char *file_path, Grid *grid);
int *encode(Line *line);
int convert_to_int(char c);
char convert_to_char(int num);
Matrix load_grid_into_matrix(Grid *g);
void print_grid(Grid *g);
Matrix adjacency_matrix(Matrix grid_matrix);
size_t *get_neighbors(Matrix *matrix, size_t row, size_t col, int *num_neighbours);

#endif // GRID_IMPLEMENTATION
