#ifndef QUEUE_IMPLEMENTATION
#define QUEUE_IMPLEMENTATION

#define QUEUE_EMPTY INT_MIN
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} Queue;

void init_queue(Queue *q);
bool enqueue(Queue *q, int value);
int dequeue(Queue *q);
void print_queue(Queue *q);
void queue_dealloc(Queue *q);


#endif // QUEUE_IMPLEMENTATION
