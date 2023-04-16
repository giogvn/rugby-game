// Standard headers
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Internal headers
#include "queue.h"
#include "position.h"

void init_queue(queue_t *q, int size)
{
    q->elements = (position_t *)malloc(size * sizeof(position_t));
    q->front = -1;
    q->rear = -1;
    q->size = size;
}

void destroy_queue(queue_t *q, int size)
{
    free(q);
}

int is_empty(queue_t *q)
{
    return (q->front == -1 && q->rear == -1);
}

int is_full(queue_t *q)
{
    return (q->rear == q->size - 1);
}

void enqueue(queue_t *q, position_t element)
{
    if (is_full(q))
    {
        printf("Queue is full\n");
        return;
    }
    else if (is_empty(q))
    {
        q->front = 0;
        q->rear = 0;
    }
    else
    {
        q->rear++;
    }
    q->elements[q->rear] = element;
}

position_t dequeue(queue_t *q)
{
    position_t element;
    if (is_empty(q))
    {
        printf("Queue is empty\n");
        position_t invalid_pos;
        invalid_pos.i = -1;
        invalid_pos.j = -1;
        return invalid_pos;
    }
    else if (q->front == q->rear)
    {
        element = q->elements[q->front];
        q->front = -1;
        q->rear = -1;
    }
    else
    {
        element = q->elements[q->front];
        q->front++;
    }
    return element;
}

position_t front(queue_t *q)
{
    if (is_empty(q))
    {
        printf("Queue is empty\n");
        position_t invalid_pos;
        invalid_pos.i = -1;
        invalid_pos.j = -1;
        return invalid_pos;
    }
    return q->elements[q->front];
}
