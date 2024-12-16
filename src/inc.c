#include "matrix.h"
#include <string.h>

#define MAX_LINE_COUNT 256
#define INIT_CAPACITY 256
#define NEW_LINE() printf("\n")

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

static Grid *grid;

void grid_alloc() {
    grid = malloc(sizeof(Grid));
    grid->items = malloc(sizeof(Line*) * INIT_CAPACITY);
    grid->count = 0;
    grid->capacity = INIT_CAPACITY;
}

void append_to_grid(Line *line) {
    if (grid->count == grid->capacity) {
        // NOTE:Reallocate memory if grid is full
        grid->capacity *= 2;
        grid->items = realloc(grid->items, grid->capacity * sizeof(Line*));
    }
    
    // NOTE: Allocate memory for the new Line and copy its contents
    grid->items[grid->count] = malloc(sizeof(Line));
    memcpy(grid->items[grid->count], line, sizeof(*line));
    grid->count++;
    grid->capacity+=1;
}

void grid_dealloc(Grid *g) {
    for (size_t i = 0; i < g->count; i++) {
        free(g->items[i]->buf);
        free(g->items[i]);
    }
    free(g->items);
    free(g);
    g = NULL;
}

bool read_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, READ_FILE_FAILED, file_path);
        return false;
    }

    char buffer[MAX_LINE_COUNT] = {0};
    while(fgets(buffer, sizeof(buffer), file)) {
        buffer[strlen(buffer) - 1] = '\0';
        int len = strlen(buffer);

        Line line;
        line.count  = 0;
        line.capacity = INIT_CAPACITY;
        line.buf = calloc(line.capacity, sizeof(char));
        if (line.buf == NULL) {
            fprintf(stderr, ALLOCATION_FAILED);
            fclose(file);
            return false;
        }

        while(line.count + len + 1 >= line.capacity) {
            line.capacity *= 2;
            char *new_buf = realloc(line.buf, line.capacity);
            if(new_buf == NULL) {
                fprintf(stderr, ALLOCATION_FAILED);
                free(line.buf);
                fclose(file);
                return false;
            }
            line.buf = new_buf;
            free(new_buf);
        }
        memcpy(line.buf + line.count, buffer , len);
        line.count += len;
        append_to_grid(&line);
    }

    fclose(file);
    return true;
}

int *encode(Line *line) {
    int *encoded = malloc(sizeof(int) * line->count);
    if (encoded == NULL) {
        fprintf(stderr, ALLOCATION_FAILED);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < line->count; i++) {
        encoded[i] = line->buf[i];
    }

    return encoded;
}

int convert(char letter) {
    return (int) letter;
}

Matrix load_grid_into_matrix(Grid *g) {
    Matrix G;
    size_t grid_cols = g->items[0]->count;
    size_t grid_rows = g->count;
    G.nrows = grid_rows;
    G.ncols = grid_cols;

    G.A = calloc(sizeof(int), grid_rows*grid_cols);
    if (G.A == NULL) {
        fprintf(stderr, ALLOCATION_FAILED);
        exit(EXIT_FAILURE);
    }
    memset(G.A, 0, grid_rows*grid_cols);

    for (size_t row = 0; row < grid_rows; ++row) {
        int *en = encode(g->items[row]);
        for (size_t col = 0; col < grid_cols; ++col) {
            SET_ELEMENT(G, row, col, en[col]);
        }
        free(en);
    }

    return G;
}

void print_grid(Grid *g) {
    for (size_t i = 0; i < g->count; ++i) {
        for (size_t j = 0; j < g->items[0]->count; ++j) {
            printf("%d ", g->items[i]->buf[j]);
        }
        NEW_LINE();
    }
}

// NOTE: DOT (.) is 46
Matrix adjacency_matrix(Matrix grid_matrix) {
    Matrix Adjacency;
    size_t weights = grid_matrix.ncols * grid_matrix.nrows;
    Adjacency.nrows = weights;
    Adjacency.ncols = weights;
    Adjacency.A = calloc(sizeof(int), weights*weights);
    if (Adjacency.A == NULL) {
        fprintf(stderr, ALLOCATION_FAILED);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < grid_matrix.nrows; ++i) {
        for (size_t j = 0; j < grid_matrix.ncols; ++j) {
            int index = i * grid_matrix.ncols + j;

            if (j + 1 < grid_matrix.ncols) {
                size_t right = index + 1;
                Adjacency.A[right * Adjacency.ncols + index] = grid_matrix.A[index];
                Adjacency.A[index * Adjacency.ncols + right] = grid_matrix.A[right];
            }

            if (j > 0) {
                size_t left = index - 1;
                Adjacency.A[left * Adjacency.ncols + index] = grid_matrix.A[index];
                Adjacency.A[index * Adjacency.ncols + left] = grid_matrix.A[left];
            }

            if (i + 1 < grid_matrix.nrows) {
                size_t down = index + grid_matrix.ncols;
                Adjacency.A[down * Adjacency.ncols + index] = grid_matrix.A[index];
                Adjacency.A[index * Adjacency.ncols + down] = grid_matrix.A[down];
            }

            if (i > 0) {
                size_t up = index - grid_matrix.ncols;
                Adjacency.A[up * Adjacency.ncols + index] = grid_matrix.A[index];
                Adjacency.A[index * Adjacency.ncols + up] = grid_matrix.A[up];
            }
        }
    }

    return Adjacency;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s file_path.\n", argv[0]);
        return 1;
    }
    
    // NOTE: Allocate Memory For Grid
    grid_alloc();
    if (grid == NULL) {
        fprintf(stderr, ALLOCATION_FAILED" For Grid");
        return 1;
    }

    // NOTE: Read File
    const char *file_path_var = argv[1];
    if(!read_file(file_path_var)) return 1;

    // NOTE: Load Grid Into Matrix
    Matrix G = load_grid_into_matrix(grid);
    PRINT(G);

    Matrix A = adjacency_matrix(G);
    // NOTE: The Adjacency data is too big to be printed(takes forever) on large files
    PRINT(A);

    int a = convert('X');
    int b = convert('S');
    printf("X is %d.\nS is %d.\n", a, b);

    // NOTE: Deallocate Allocated memory
    UNLOAD(&G);
    UNLOAD(&A);
    grid_dealloc(grid);
    return 0;
}
