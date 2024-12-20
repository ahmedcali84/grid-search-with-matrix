#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <assert.h>

#define MATDEF static __attribute__((unused)) // Define a macro for static functions and suppresses all warnings assosiated with unused functions defined with MATDEF

typedef enum {
    TYPE_BOOL,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_SIZE_T,
    TYPE_CHAR,
} Element_Type;

// Matrix Structure and Information
typedef struct {
    void *A;             // Pointer to the array of matrix elements
    size_t nrows;        // Number of rows in the matrix
    size_t ncols;        // Number of columns in the matrix
    size_t element_size; // Element Size
    Element_Type type;   // Element Type
} Matrix;

/*
************** MATRIX OPERATIONS FUNCTIONS DECLARATIONS *******************
*/

MATDEF void GET_ELEMENT(Matrix C, size_t row, size_t col, void *target);                                   // Get Specific Element from the Matrix
MATDEF void SET_ELEMENT(Matrix C, size_t row, size_t col, void *value);                                    // Set a specific element from the matrix to a new value
MATDEF Matrix CREATE_MATRIX(size_t rows, size_t cols, size_t element_size, Element_Type type);             // Creates an empty Matrix With specified parameters
MATDEF Matrix RANDOM_MATRIX(size_t nrows, size_t ncols, size_t element_size, Element_Type type);           // Generates random Matrix(nrows x ncols)
MATDEF void PRINT_MATRIX(const Matrix B, const char *name);                                                      // Prints a Matrix
MATDEF int MATRIX_SHAPE(Matrix A, const char *name);                                                      // Prints the SHAPE of the matrix
MATDEF Matrix MATRIX_ADD(Matrix *A, Matrix *B);                                                            // Adds two Matrices
MATDEF Matrix MATRIX_SUBTRACT(Matrix *A, Matrix *B);                                                       // Subtracts two Matrices
MATDEF Matrix HADAMARD_PRODUCT(Matrix *A, Matrix *B);                                                      // Computes Element-wise Product of Two Matrices
MATDEF Matrix DOT_PRODUCT(Matrix *A, Matrix *B);                                                          // Computes the Dot-Product of two Matrices
MATDEF Matrix TRANSPOSE(Matrix *A);                                                                        // Return TRANSPOSE of Matrix A
MATDEF bool TEST_MATRIX_EQUAL(Matrix A, Matrix B, char *matrix_a, char *matrix_b);                         // Test Matrices for equality
MATDEF Matrix FILL(size_t nrows, size_t ncols, size_t element_size, Element_Type type, void *fill_value);  // Creates A Matrix filled with a specific value (For Development Purposes)
MATDEF void UNLOAD(Matrix *B);                                                                             // Function to free Allocated Memory

// Special Macro Functions
#define PRINT(B) PRINT_MATRIX(B, #B)                                                                       // Macro definition of a special print function
#define SHAPE(A) MATRIX_SHAPE(A, #A)                                                                       // Macro definition of a special SHAPE printing function
#define TEST_MATRIX(A, B) TEST_MATRIX_EQUAL(A, B, #A, #B)                                                  // Special Macro Version of Test Matrix Function also prints the variable-name of the Matrix
#define ALLOCATION_FAILED "Memory Allocation Failed.\n"


/*
************** MATRIX OPERATIONS FUNCTIONS IMPLEMENTATIONS *******************
*/

MATDEF void GET_ELEMENT(Matrix C, size_t row, size_t col, void *target) {
    // Check for valid indices
    if (row >= C.nrows || col >= C.ncols) {
        fprintf(stderr, "Index out of bounds: [%zu, %zu] for matrix size [%zu, %zu]\n",
                row, col, C.nrows, C.ncols);
        exit(EXIT_FAILURE); // Handle the error, exit or return an error value
    }

    // Retrieve an element from the matrix at the specified row and column
    void *element = (char *) C.A + (row * C.ncols + col) * C.element_size;
    memcpy(target, element, C.element_size);
}

MATDEF void SET_ELEMENT(Matrix C, size_t row, size_t col, void *value) {
    // Set the element at the specified row and column to a new value
    void *element = (char *) C.A + (row * C.ncols + col) * C.element_size;
    memcpy(element, value, C.element_size);
}

Matrix CREATE_MATRIX(size_t rows, size_t cols, size_t element_size, Element_Type type) {
    Matrix create;
    create.nrows = rows;
    create.ncols = cols;
    create.element_size = element_size;
    create.type = type;
    create.A = calloc(element_size, (rows * cols));
    if (create.A == NULL) {
        fprintf(stderr, ALLOCATION_FAILED);
        exit(EXIT_FAILURE);
    }

    return create;
}

MATDEF Matrix RANDOM_MATRIX(size_t nrows, size_t ncols, size_t element_size, Element_Type type) {
    // Initialize the Matrix
    Matrix random = CREATE_MATRIX(nrows, ncols, element_size, type);

    // Fill the matrix with random values
    switch (random.type) {
        case TYPE_INT:
            for (size_t i = 0; i < random.nrows; ++i) {
                for (size_t j = 0; j < random.ncols; ++j) {
                    int v = (rand() % 21) - 10;
                    SET_ELEMENT(random, i , j , &v);
                }
            }
            break;

        case TYPE_BOOL:
            for (size_t i = 0; i < random.nrows; ++i) {
                for (size_t j = 0; j < random.ncols; ++j) {
                    bool v = rand() % 2;
                    SET_ELEMENT(random, i , j , &v);
                }
            }
            break;

        case TYPE_DOUBLE:
            for (size_t i = 0; i < random.nrows; ++i) {
                for (size_t j = 0; j < random.ncols; ++j) {
                    double v = ((double)rand() / RAND_MAX) * 20.0 - 10.0;
                    SET_ELEMENT(random, i , j , &v);
                }
            }
            break;

        case TYPE_SIZE_T:
            for (size_t i = 0; i < random.nrows; ++i) {
                for (size_t j = 0; j < random.ncols; ++j) {
                    size_t v = rand() % 21;
                    SET_ELEMENT(random, i , j , &v);
                }
            }
            break;

        case TYPE_CHAR:
            for (size_t i = 0; i < random.nrows; ++i) {
                for (size_t j = 0; j < random.ncols; ++j) {
                    char v = 'A' + rand() % 26;
                    SET_ELEMENT(random, i , j , &v);
                }
            }
            break;

        case TYPE_FLOAT:
            for (size_t i = 0; i < random.nrows; ++i) {
                for (size_t j = 0; j < random.ncols; ++j) {
                    float v = ((float) rand() / RAND_MAX) * 20.0f - 10.0f;
                    SET_ELEMENT(random, i , j , &v);
                }
            }
            break;

        default:
            fprintf(stderr, "Unknown Type for the type Matrix");
            exit(EXIT_FAILURE);
    }

    // Return the random matrix
    return random; // Return the generated matrix
}

MATDEF void PRINT_MATRIX(const Matrix B, const char *name) {
    // Print the matrix with its name
    printf("%s = [\n", name);
    switch (B.type) {
        case TYPE_INT:
            for (size_t i = 0; i < B.nrows; ++i) {
                for (size_t j = 0; j < B.ncols; ++j) {
                    int v;
                    GET_ELEMENT(B, i , j , &v);
                    printf(" %d ", v);
                }
                printf("\n");
            }
            break;

        case TYPE_BOOL:
            for (size_t i = 0; i < B.nrows; ++i) {
                for (size_t j = 0; j < B.ncols; ++j) {
                    bool v;;
                    GET_ELEMENT(B, i , j , &v);
                    printf(" %s ", v ? "true ": "false");
                }
                printf("\n");
            }
            break;

        case TYPE_DOUBLE:
            for (size_t i = 0; i < B.nrows; ++i) {
                for (size_t j = 0; j < B.ncols; ++j) {
                    double v;
                    GET_ELEMENT(B, i , j , &v);
                    printf(" %.2f ", v);
                }
                printf("\n");
            }
            break;

        case TYPE_SIZE_T:
            for (size_t i = 0; i < B.nrows; ++i) {
                for (size_t j = 0; j < B.ncols; ++j) {
                    size_t v;
                    GET_ELEMENT(B, i , j , &v);
                    printf(" %zu ", v);
                }
                printf("\n");
            }
            break;

        case TYPE_CHAR:
            for (size_t i = 0; i < B.nrows; ++i) {
                for (size_t j = 0; j < B.ncols; ++j) {
                    char v;
                    GET_ELEMENT(B, i , j , &v);
                    printf(" %c ",v);
                }
                printf("\n");
            }
            break;

        case TYPE_FLOAT:
            for (size_t i = 0; i < B.nrows; ++i) {
                for (size_t j = 0; j < B.ncols; ++j) {
                    float v;
                    GET_ELEMENT(B, i , j , &v);
                    printf(" %.2f ", v);
                }
                printf("\n");
            }
            break;

        default:
            fprintf(stderr, "Unknown Type Selected For Printing");
            exit(EXIT_FAILURE);
    } 
    printf("]"); // End of matrix
    printf("\n\n");
}

MATDEF int MATRIX_SHAPE(Matrix A, const char *name) {
    // Print the shape of the matrix (rows, columns) and its type
    switch (A.type) {
        case TYPE_FLOAT:
            printf("%s(SHAPE: (%zu , %zu), dtype=float)\n", name, A.nrows, A.ncols);
            break;
        case TYPE_SIZE_T:
            printf("%s(SHAPE: (%zu , %zu), dtype=size_t)\n", name, A.nrows, A.ncols);
            break;
        case TYPE_INT:
            printf("%s(SHAPE: (%zu , %zu), dtype=int)\n", name, A.nrows, A.ncols);
            break;
        case TYPE_BOOL:
            printf("%s(SHAPE: (%zu , %zu), dtype=bool)\n", name, A.nrows, A.ncols);
            break;
        case TYPE_DOUBLE:
            printf("%s(SHAPE: (%zu , %zu), dtype=double)\n", name, A.nrows, A.ncols);
            break;
        case TYPE_CHAR:
            printf("%s(SHAPE: (%zu , %zu), dtype=char)\n", name, A.nrows, A.ncols);
            break;
        default:
            printf("Unknown Type.\n");
            return 1;
    }
    return 0;
}

MATDEF Matrix MATRIX_ADD(Matrix *A, Matrix *B) {
    // Check if the Dimensions of the Matrices are the same
    assert((A->ncols == B->ncols && B->nrows == A->nrows && A->type == B->type) && "Addition Failed, Matrices Shape Mismatch");

    // Initialize a new matrix for the result
    Matrix C = CREATE_MATRIX(A->nrows, A->ncols, A->element_size, A->type);
    
    switch (A->type) {
        case TYPE_SIZE_T:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    size_t a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a + b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_FLOAT:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    float a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a + b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_CHAR:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    char a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a + b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_INT:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    int a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a + b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_DOUBLE:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    double a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a + b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_BOOL:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    bool a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a || b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        default:
            fprintf(stderr, "Unknown Type For Addition Matrices");
            exit(EXIT_FAILURE);
    }

    return C; // Return the result matrix
}

MATDEF Matrix MATRIX_SUBTRACT(Matrix *A, Matrix *B) {
    // Check if the Dimensions of the Matrices are the same
    assert((A->ncols == B->ncols && B->nrows == A->nrows && A->type == B->type) && "Subtraction Failed, Matrices Shape Mismatch");

    // Initialize a new matrix for the result
    Matrix C = CREATE_MATRIX(A->nrows, A->ncols, A->element_size, A->type);

    switch (A->type) {
        case TYPE_SIZE_T:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    size_t a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a - b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_FLOAT:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    float a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a - b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_CHAR:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    char a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a - b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_INT:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    int a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a - b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_DOUBLE:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    double a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a - b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_BOOL:
            printf("ERROR: Subtraction of boolean not Supported Contribute to the library if You have An Idea About it.\n");
            exit(EXIT_FAILURE);

        default:
            fprintf(stderr, "Unknown Type For Subtraction Matrices");
            exit(EXIT_FAILURE);
    }

    return C; // Return the result
}

MATDEF Matrix HADAMARD_PRODUCT(Matrix *A, Matrix *B) {
    // Ensure matrices are of the same size and type
    assert(A->ncols == B->ncols && "Matrices Dimensions Mismatch");
    assert(A->nrows == B->nrows && "Matrices Dimensions Mismatch");
    assert(A->type  == A->type  && "Type Mismatch");

    // Initialize a new matrix for the result
    Matrix C = CREATE_MATRIX(A->nrows, A->ncols, A->element_size, A->type);

    // Calculate element-wise product
    switch (A->type) {
        case TYPE_SIZE_T:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    size_t a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a * b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_FLOAT:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    float a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a * b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_CHAR:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    char a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a * b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_INT:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    int a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a * b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_DOUBLE:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    double a , b , c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a * b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        case TYPE_BOOL:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    bool a , b, c;
                    GET_ELEMENT(*A, i , j , &a);
                    GET_ELEMENT(*B, i , j , &b);
                    c = a && b;
                    SET_ELEMENT(C, i , j , &c);
                }
            }
            break;

        default:
            fprintf(stderr, "Unknown Type For Multiplication Matrices");
            exit(EXIT_FAILURE);
    }
    

    return C; // Return the result matrix
}

MATDEF Matrix DOT_PRODUCT(Matrix *A, Matrix *B) {
    // Check if the matrices can be multiplied
    assert((A->ncols == B->nrows) && "Error Multiplying: Dimensions Mismatch");
    assert((A->type == B->type) && "Type Mismatch");

    // Initialize a new matrix for the result
    Matrix C = CREATE_MATRIX(A->nrows, B->ncols, A->element_size, A->type);

    // Calculate the dot product
    switch (A->type) {
        case TYPE_FLOAT:
            for (size_t i = 0; i < C.nrows; ++i) {
                for (size_t j = 0; j < C.ncols; ++j) {
                    float c = 0;
                    for (size_t k = 0; k < A->ncols; ++k) {
                        float a , b;
                        GET_ELEMENT(*A, i , k , &a);
                        GET_ELEMENT(*B, k , j , &b);
                        c += a * b;
                    }
                    SET_ELEMENT(C , i , j , &c);
                }
            }
            break;

        case TYPE_INT:
            for (size_t i = 0; i < C.nrows; ++i) {
                for (size_t j = 0; j < C.ncols; ++j) {
                    int c = 0;
                    for (size_t k = 0; k < A->ncols; ++k) {
                        int a , b;
                        GET_ELEMENT(*A, i , k , &a);
                        GET_ELEMENT(*B, k , j , &b);
                        c += a * b;
                    }
                    SET_ELEMENT(C , i , j , &c);
                }
            }
            break;

        case TYPE_DOUBLE:
            for (size_t i = 0; i < C.nrows; ++i) {
                for (size_t j = 0; j < C.ncols; ++j) {
                    double c = 0;
                    for (size_t k = 0; k < A->ncols; ++k) {
                        double a , b;
                        GET_ELEMENT(*A, i , k , &a);
                        GET_ELEMENT(*B, k , j , &b);
                        c += a * b;
                    }
                    SET_ELEMENT(C , i , j , &c);
                }
            }
            break;

        case TYPE_SIZE_T:
            for (size_t i = 0; i < C.nrows; ++i) {
                for (size_t j = 0; j < C.ncols; ++j) {
                    size_t c = 0;
                    for (size_t k = 0; k < A->ncols; ++k) {
                        size_t a , b;
                        GET_ELEMENT(*A, i , k , &a);
                        GET_ELEMENT(*B, k , j , &b);
                        c += a * b;
                    }
                    SET_ELEMENT(C , i , j , &c);
                }
            }
            break;

        case TYPE_BOOL:
            for (size_t i = 0; i < C.nrows; ++i) {
                for (size_t j = 0; j < C.ncols; ++j) {
                    bool c = false;
                    for (size_t k = 0; k < A->ncols; ++k) {
                        float a , b;
                        GET_ELEMENT(*A, i , k , &a);
                        GET_ELEMENT(*B, k , j , &b);
                        c = c || (a && b);
                    }
                    SET_ELEMENT(C , i , j , &c);
                }
            }
            break;

        case TYPE_CHAR:
            for (size_t i = 0; i < C.nrows; ++i) {
                for (size_t j = 0; j < C.ncols; ++j) {
                    char c = 0;
                    for (size_t k = 0; k < A->ncols; ++k) {
                        char a , b;
                        GET_ELEMENT(*A, i , k , &a);
                        GET_ELEMENT(*B, k , j , &b);
                        c += a * b;
                    }
                    SET_ELEMENT(C , i , j , &c);
                }
            }
            break;

        default:
            printf("ERROR: Unknown Type For the Multiplication Operations.\n");
            exit(EXIT_FAILURE);
    }

    return C; // Return the result matrix
}

MATDEF Matrix TRANSPOSE(Matrix *A) {
    // Initialize a new matrix for the result
    Matrix C = CREATE_MATRIX(A->ncols, A->nrows, A->element_size, A->type);

    // Calculate the transpose
    switch (A->type) {
        case TYPE_INT:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    int a;
                    GET_ELEMENT(*A, i , j , &a);
                    SET_ELEMENT(C, j , i , &a);
                }
            }
            break;

        case TYPE_FLOAT:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    float a;
                    GET_ELEMENT(*A, i , j , &a);
                    SET_ELEMENT(C, j , i , &a);
                }
            }
            break;

        case TYPE_DOUBLE:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    double a;
                    GET_ELEMENT(*A, i , j , &a);
                    SET_ELEMENT(C, j , i , &a);
                }
            }
            break;

        case TYPE_CHAR:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    char a;
                    GET_ELEMENT(*A, i , j , &a);
                    SET_ELEMENT(C, j , i , &a);
                }
            }
            break;

        case TYPE_SIZE_T:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    size_t a;
                    GET_ELEMENT(*A, i , j , &a);
                    SET_ELEMENT(C, j , i , &a);
                }
            }
            break;

        case TYPE_BOOL:
            for (size_t i = 0; i < A->nrows; ++i) {
                for (size_t j = 0; j < A->ncols; ++j) {
                    bool a;
                    GET_ELEMENT(*A, i , j , &a);
                    SET_ELEMENT(C, j , i , &a);
                }
            }
            break;

        default:
            printf("ERROR: Unknown Type For Transpose Operation.\n");
            exit(EXIT_FAILURE);
    }

    return C; // Return the transposed matrix
}

MATDEF bool TEST_MATRIX_EQUAL(Matrix A, Matrix B, char *matrix_a, char *matrix_b) {
    // Check if the dimensions match
    assert(((A.ncols == B.ncols) || (A.nrows == B.nrows)) && (A.type == B.type) && "NOT EQUAL: Different Dimensions.");

    // Check if all elements are equal
    switch (A.type) {
        case TYPE_FLOAT:
            for (size_t i = 0; i < A.nrows; ++i) {
                for (size_t j = 0; j < A.ncols; ++j) {
                    float a,  b;
                    GET_ELEMENT(A, i, j, &a);
                    GET_ELEMENT(B, i, j, &b); 
                    if (a != b) {
                        printf("%s and %s are NOT EQUAL: Values differ at (%zu, %zu)\n", matrix_a, matrix_b, i, j); // Print mismatch
                        return false; // Not equal
                    }
                }
            }
            break;

        case TYPE_INT:
            for (size_t i = 0; i < A.nrows; ++i) {
                for (size_t j = 0; j < A.ncols; ++j) {
                    int a , b;
                    GET_ELEMENT(A, i, j, &a);
                    GET_ELEMENT(B, i, j, &b); 
                    if (a != b) {
                        printf("%s and %s are NOT EQUAL: Values differ at (%zu, %zu)\n", matrix_a, matrix_b, i, j); // Print mismatch
                        return false; // Not equal
                    }
                }
            }
            break;

        case TYPE_SIZE_T:
            for (size_t i = 0; i < A.nrows; ++i) {
                for (size_t j = 0; j < A.ncols; ++j) {
                    size_t a , b;
                    GET_ELEMENT(A, i, j, &a);
                    GET_ELEMENT(B, i, j, &b); 
                    if (a != b) {
                        printf("%s and %s are NOT EQUAL: Values differ at (%zu, %zu)\n", matrix_a, matrix_b, i, j); // Print mismatch
                        return false; // Not equal
                    }
                }
            }
            break;

        case TYPE_CHAR:
            for (size_t i = 0; i < A.nrows; ++i) {
                for (size_t j = 0; j < A.ncols; ++j) {
                    char a , b;
                    GET_ELEMENT(A, i, j, &a);
                    GET_ELEMENT(B, i, j, &b); 
                    if (a != b) {
                        printf("%s and %s are NOT EQUAL: Values differ at (%zu, %zu)\n", matrix_a, matrix_b, i, j); // Print mismatch
                        return false; // Not equal
                    }
                }
            }
            break;


        case TYPE_DOUBLE:
            for (size_t i = 0; i < A.nrows; ++i) {
                for (size_t j = 0; j < A.ncols; ++j) {
                    double a , b;
                    GET_ELEMENT(A, i, j, &a);
                    GET_ELEMENT(B, i, j, &b); 
                    if (a != b) {
                        printf("%s and %s are NOT EQUAL: Values differ at (%zu, %zu)\n", matrix_a, matrix_b, i, j); // Print mismatch
                        return false; // Not equal
                    }
                }
            }
            break;
        
        case TYPE_BOOL:
            for (size_t i = 0; i < A.nrows; ++i) {
                for (size_t j = 0; j < A.ncols; ++j) {
                    bool a , b;
                    GET_ELEMENT(A, i, j, &a);
                    GET_ELEMENT(B, i, j, &b); 
                    if (a != b) {
                        printf("%s and %s are NOT EQUAL: Values differ at (%zu, %zu)\n", matrix_a, matrix_b, i, j); // Print mismatch
                        return false; // Not equal
                    }
                }
            }
            break;

        default:
            printf("Unknown Type For Identifying Equal Matrix.\n");
            exit(EXIT_FAILURE);
    }

    printf("%s and %s are EQUAL\n", matrix_a, matrix_b); // Print equal matrices
    return true; // Equal
}

MATDEF Matrix FILL(size_t nrows, size_t ncols, size_t element_size, Element_Type type, void *fill_value) {
    // Initialize matrix
    Matrix A = CREATE_MATRIX(nrows, ncols, element_size, type);

    // Fill the matrix with the specified value
    for (size_t i = 0; i < nrows; ++i) {
        for (size_t j = 0; j < ncols; ++j) {
            SET_ELEMENT(A, i , j , fill_value); // Set all elements
        }
    }

    return A; // Return the filled matrix
}

MATDEF void UNLOAD(Matrix *B) {
    // Free allocated memory for the matrix
    if (B->A != NULL)
    {
        free(B->A); // Free the array of elements
        B->A = NULL; // Avoid dangling pointer
    }
    B->nrows = 0; // Reset rows
    B->ncols = 0; // Reset columns
}

#endif // MATRIX_H
