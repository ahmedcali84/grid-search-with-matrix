#include "grid.h"

// Intialize the dir for neighbors (left, right, up, down, diagonal)
int dir[8][2] = {
    {-1, 0}, // Up
    {1, 0},  // Down
    {0, -1}, // Left
    {0, 1},  // Right
    {-1, -1}, // Top-left diagonal
    {-1, 1},  // Top-right diagonal
    {1, -1},  // Bottom-left diagonal
    {1, 1}    // Bottom-right diagonal
};

Grid *grid_alloc(Grid *grid) {
    grid = malloc(sizeof(Grid));
    grid->items = malloc(sizeof(Line*) * INIT_CAPACITY);
    grid->count = 0;
    grid->capacity = INIT_CAPACITY;
    return grid;
}

void append_to_grid(Grid *grid, Line *line) {
    if (grid->count == grid->capacity) {
        // NOTE: Reallocate memory if grid is full
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

bool read_file(const char *file_path, Grid *grid) {
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
        append_to_grid(grid, &line);
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

int convert_to_int(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A'; // Map 'A' to 0, 'B' to 1, ..., 'Z' to 25
    } else {
        fprintf(stderr, "Invalid character '%c' encountered.\n", c);
        return -1; // Indicate invalid character
    }
}

char convert_to_char(int num) {
    return (char) num;
}

Matrix load_grid_into_matrix(Grid *g) {
    size_t grid_cols = g->items[0]->count;
    size_t grid_rows = g->count;
    Matrix G = CREATE_MATRIX(grid_rows, grid_cols, sizeof(int), TYPE_INT);

    for (size_t row = 0; row < grid_rows; ++row) {
        int *en = encode(g->items[row]);
        for (size_t col = 0; col < grid_cols; ++col) {
            int n = en[col];
            SET_ELEMENT(G, row, col, &n);
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

Matrix adjacency_matrix(Matrix grid_matrix) {
    size_t weights = grid_matrix.ncols * grid_matrix.nrows;
    Matrix Adjacency = CREATE_MATRIX(weights, weights, sizeof(bool), TYPE_BOOL);
    bool a = true;

    for (size_t i = 0; i < grid_matrix.nrows; ++i) {
        for (size_t j = 0; j < grid_matrix.ncols; ++j) {
            int index = i * grid_matrix.ncols + j;

            if (i > 0 && j + 1 < grid_matrix.ncols) {
                size_t diagonal_right_up = index - grid_matrix.ncols + 1;
                SET_ELEMENT(Adjacency, diagonal_right_up, index, &a);
                SET_ELEMENT(Adjacency, index, diagonal_right_up, &a);
            }

            if (j + 1 < grid_matrix.ncols) {
                size_t right = index + 1;
                SET_ELEMENT(Adjacency, right, index, &a);
                SET_ELEMENT(Adjacency, index, right, &a);
            }

            if (i + 1 < grid_matrix.nrows && j + 1 < grid_matrix.ncols) {
                size_t diagonal_right_down = index + grid_matrix.ncols + 1;
                SET_ELEMENT(Adjacency, diagonal_right_down, index, &a);
                SET_ELEMENT(Adjacency, index, diagonal_right_down, &a);
            }

            if (i > 0 && j > 0) {
                size_t diagonal_left_up = (i - 1) * grid_matrix.ncols + (j - 1);
                SET_ELEMENT(Adjacency, diagonal_left_up, index, &a);
                SET_ELEMENT(Adjacency, index, diagonal_left_up, &a);
            }

            if (j > 0) {
                size_t left = index - 1;
                SET_ELEMENT(Adjacency, left, index, &a);
                SET_ELEMENT(Adjacency, index, left, &a);
            }

            if (i + 1 < grid_matrix.nrows && j > 0) {
                size_t diagonal_left_down = (i + 1) * grid_matrix.ncols + (j - 1);
                SET_ELEMENT(Adjacency, diagonal_left_down, index, &a);
                SET_ELEMENT(Adjacency, index, diagonal_left_down, &a);
            }

            if (i + 1 < grid_matrix.nrows) {
                size_t down = index + grid_matrix.ncols;
                SET_ELEMENT(Adjacency, down, index, &a);
                SET_ELEMENT(Adjacency, index, down, &a);
            }

            if (i > 0) {
                size_t up = index - grid_matrix.ncols;
                SET_ELEMENT(Adjacency, up, index, &a);
                SET_ELEMENT(Adjacency, index, up, &a);
            }
        }
    }

    return Adjacency;
}

size_t *get_neighbors(Matrix *matrix, size_t row, size_t col, int *num_neighbours) {
    // Allocate memory for up to 8 neighbors
    size_t *valid_neighbours = malloc(sizeof(size_t) * 8);
    if (valid_neighbours == NULL) {
        fprintf(stderr, ALLOCATION_FAILED);
        exit(EXIT_FAILURE);
    }

    int count = 0;

    // Explore all 8 directions
    for (int i = 0; i < 8; i++) {
        int newRow = row + dir[i][0];
        int newCol = col + dir[i][1];

        // Check if the new row and column are within the bounds of the matrix
        if (newRow >= 0 && newRow < (int)matrix->nrows && newCol >= 0 && newCol < (int)matrix->ncols) {
            valid_neighbours[count] = newRow * matrix->ncols + newCol;
            count++;
        }
    }

    // Resize the array to match the actual number of valid neighbors
    size_t *new_neighbours = realloc(valid_neighbours, count * sizeof(size_t));
    if (new_neighbours == NULL) {
        fprintf(stderr, ALLOCATION_FAILED);
        free(valid_neighbours);
        exit(EXIT_FAILURE);
    }

    valid_neighbours = new_neighbours;
    *num_neighbours = count;  // Set the actual number of neighbors

    return valid_neighbours;
}
