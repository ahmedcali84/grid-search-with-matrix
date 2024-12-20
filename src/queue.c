#include "queue.h"

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
