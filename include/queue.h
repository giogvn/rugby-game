#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

// Internal headers
#include "position.h"

struct queue_node
{
    position_t data;
    struct queue_node *next;
};

struct queue
{
    struct queue_node *head;
    struct queue_node *tail;
};
typedef struct queue queue_t;

void init_queue(queue_t *q);
void destroy_queue(queue_t *q);
int is_empty(queue_t *q);
int is_full(queue_t *q);
void enqueue(queue_t *q, position_t element);
position_t dequeue(queue_t *q);

#endif
