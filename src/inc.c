#include "matrix.h"
#include <string.h>

#define MAX_LINE_COUNT 256
#define INIT_CAPACITY 256
#define NEW_LINE() printf("\n")
#define QUEUE_EMPTY INT_MIN
#define UNUSED(x) ((void) x)

// Define the directions for neighbors (left, right, up, down, diagonal)
int directions[8][2] = {
    {-1, 0}, // Up
    {1, 0},  // Down
    {0, -1}, // Left
    {0, 1},  // Right
    {-1, -1}, // Top-left diagonal
    {-1, 1},  // Top-right diagonal
    {1, -1},  // Bottom-left diagonal
    {1, 1}    // Bottom-right diagonal
};

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} Queue;

void init_queue(Queue *q) {
    q->head = NULL;
    q->tail = NULL;
}

bool enqueue(Queue *q, int value) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) return false;
    node->value = value;
    node->next = NULL;

    if (q->tail != NULL) {
        q->tail->next = node;
    }
    q->tail = node;

    if (q->head == NULL) {
        q->head = node;
    }
    return true;
}

int dequeue(Queue *q) {
    if (q->head == NULL) return QUEUE_EMPTY;

    Node *temp = q->head;
    int result = temp->value;
    q->head = q->head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    
    free(temp);
    return result;
}

void print_queue(Queue *q) {
    if (q == NULL || q->head == NULL) {
        printf("Queue is Empty or uninitialized.\n");
        return;
    }

    Node *tmp = q->head;
    while(tmp != NULL) {
        if (tmp->value != QUEUE_EMPTY) {
            printf("%d ", tmp->value);
        }
        //printf("\n");
        tmp = tmp->next;
    }
    printf("\n");
}

void queue_dealloc(Queue *q) {
    Node *current = q->head;
    Node *nextnode;

    while(current != NULL) {
        nextnode = current->next;
        free(current);
        current = nextnode;
    }
}

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

int convert_to_int(char c) {
    return (int) c;
}

char convert_to_char(int num) {
    return (char) num;
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

            if (i > 0 && j + 1 < grid_matrix.ncols) {
                size_t diagonal_right_up = index - grid_matrix.ncols + 1;
                SET_ELEMENT(Adjacency, diagonal_right_up, index, (int) 1);
                SET_ELEMENT(Adjacency, index, diagonal_right_up, (int) 1);
            }

            if (j + 1 < grid_matrix.ncols) {
                size_t right = index + 1;
                SET_ELEMENT(Adjacency, right, index, (int) 1);
                SET_ELEMENT(Adjacency, index, right, (int) 1);
            }

            if (i + 1 < grid_matrix.nrows && j + 1 < grid_matrix.ncols) {
                size_t diagonal_right_down = index + grid_matrix.ncols + 1;
                SET_ELEMENT(Adjacency, diagonal_right_down, index, (int) 1);
                SET_ELEMENT(Adjacency, index, diagonal_right_down, (int) 1);
            }

            if (i > 0 && j > 0) {
                size_t diagonal_left_up = (i - 1) * grid_matrix.ncols + (j - 1);
                SET_ELEMENT(Adjacency, diagonal_left_up, index, (int) 1);
                SET_ELEMENT(Adjacency, index, diagonal_left_up, (int) 1);
            }

            if (j > 0) {
                size_t left = index - 1;
                SET_ELEMENT(Adjacency, left, index, (int) 1);
                SET_ELEMENT(Adjacency, index, left, (int) 1);
            }

            if (i + 1 < grid_matrix.nrows && j > 0) {
                size_t diagonal_left_down = (i + 1) * grid_matrix.ncols + (j - 1);
                SET_ELEMENT(Adjacency, diagonal_left_down, index, (int) 1);
                SET_ELEMENT(Adjacency, index, diagonal_left_down, (int) 1);
            }

            if (i + 1 < grid_matrix.nrows) {
                size_t down = index + grid_matrix.ncols;
                SET_ELEMENT(Adjacency, down, index, (int) 1);
                SET_ELEMENT(Adjacency, index, down, (int) 1);
            }

            if (i > 0) {
                size_t up = index - grid_matrix.ncols;
                SET_ELEMENT(Adjacency, up, index, (int) 1);
                SET_ELEMENT(Adjacency, index, up, (int) 1);
            }
        }
    }

    return Adjacency;
}

int solve(char Start, char End, Matrix *Adjacency) {    
    bool *visited = malloc(sizeof(bool) * (Adjacency->ncols));
    if (visited == NULL) {
        fprintf(stderr, ALLOCATION_FAILED);
        exit(EXIT_FAILURE);
    }
    memset(visited, 0, (sizeof(bool)*(Adjacency->ncols)));

    int move_count = 0;
    int nodes_in_next_layer = 0;
    int nodes_left_in_layer = 1;

    Queue index;
    init_queue(&index);
    enqueue(&index, convert_to_int(Start));
    visited[convert_to_int(Start)] = true;

    while (index.head != NULL) {
        int id = dequeue(&index);
        nodes_left_in_layer--;

        if (id == convert_to_int(End)) {
            free(visited);
            return move_count;
        }
        
        for (size_t neighbour = 0; neighbour < Adjacency->ncols; ++neighbour) {
            if (GET_ELEMENT(*Adjacency, id, neighbour) && !visited[neighbour]) {
                enqueue(&index, neighbour);
                visited[neighbour] = true;
                nodes_in_next_layer++;
            }
        }

        if (nodes_left_in_layer == 0) {
            nodes_left_in_layer = nodes_in_next_layer;
            nodes_in_next_layer = 0;
            move_count++;
        }
        // print_queue(&index);
    }

    free(visited);
    return -1;
}

size_t *get_neighbors(Matrix *matrix, size_t row, size_t col, int *num_neighbours) {
    size_t *valid_neighbours = malloc(sizeof(size_t) * 8);
    if (valid_neighbours == NULL) {
        fprintf(stderr, ALLOCATION_FAILED);
        exit(EXIT_FAILURE);
    }

    int count = 0;

    for (int i = 0; i < 8; i++) {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];

        // Check if the new row and column are within the bounds of the matrix
        if (newRow >= 0 && newRow < (int)matrix->nrows && newCol >= 0 && newCol < (int)matrix->ncols) {
            valid_neighbours[count] = newRow * matrix->ncols + newCol;
            count++;
        }
    }

    // Resize array to the actual number of neighbors
    size_t *new_neighbours = realloc(valid_neighbours, count * sizeof(size_t));
    if (new_neighbours == NULL) {
        fprintf(stderr, ALLOCATION_FAILED);
        free(valid_neighbours);
        exit(EXIT_FAILURE);
    }
    valid_neighbours = new_neighbours;
    *num_neighbours = count;
    return valid_neighbours;
}

void dfs(Matrix *G, char Start, bool *visited, char End) {
    visited[convert_to_int(Start)] = true;
    printf("Visited Node %d\n", Start);

    if (Start == End) {
        printf("Target Reached.\n");
        return;
    }

    for (size_t i = 0; i < G->ncols; ++i) {
        for (size_t j = 0; j < G->ncols; ++j) {
            int num_neighbours = 0;
            size_t *valid = get_neighbors(G, i , j , &num_neighbours);
            for (int k = 0; k < num_neighbours; ++k) {
                if (!visited[(int)G->A[valid[k]]]) {
                    dfs(G, G->A[k], visited, End);
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s file_path StartAndEndJoined.\n", argv[0]);
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

    char Start = argv[2][0];
    char End = argv[2][1];
    printf("Start: %c is %d\nEnd: %c is %d\n", argv[2][0], argv[2][0], argv[2][1], argv[2][1]);
    UNUSED(Start);
    UNUSED(End);
    
    bool *visited = malloc(sizeof(bool) * (A.ncols));
    if (visited == NULL) {
        fprintf(stderr, ALLOCATION_FAILED);
        exit(EXIT_FAILURE);
    }
    memset(visited, 0, (sizeof(bool)*(A.ncols)));

    // dfs(&G, Start, visited, End);

    // NOTE: Deallocate Allocated memory
    UNLOAD(&G);
    UNLOAD(&A);
    // queue_dealloc(&queue);
    grid_dealloc(grid);
    return 0;
}
