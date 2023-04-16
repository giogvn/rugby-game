#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

// Internal headers
#include "position.h"

struct queue
{
    position_t *elements;
    int front;
    int rear;
    int size;
};
typedef struct queue queue_t;

void init_queue(queue_t *q, int size);
void destroy_queue(queue_t *q, int size);
int is_empty(queue_t *q);
int is_full(queue_t *q);
void enqueue(queue_t *q, position_t element);
position_t dequeue(queue_t *q);
position_t front(queue_t *q);

#endif
