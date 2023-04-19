// Standard headers
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Internal headers
#include "queue.h"
#include "position.h"

void init_queue(queue_t *q)
{
    q->head = NULL;
    q->tail = NULL;
}

void enqueue(queue_t *q, position_t data)
{
    struct queue_node *new_node = (struct queue_node *)malloc(sizeof(struct queue_node));
    new_node->data = data;
    new_node->next = NULL;
    if (q->tail == NULL)
    {
        q->head = new_node;
    }
    else
    {
        q->tail->next = new_node;
    }
    q->tail = new_node;
}
void destroy_queue(queue_t *q)
{
    while (!is_empty(q))
    {
        dequeue(q);
    }
}
position_t dequeue(queue_t *q)
{
    position_t data;
    if (q->head == NULL)
    {
        printf("Error: Queue is empty\n");
        data.i = -1;
        data.j = -1;
        return data;
    }
    struct queue_node *head_node = q->head;
    data = head_node->data;
    q->head = head_node->next;
    free(head_node);
    if (q->head == NULL)
    {
        q->tail = NULL;
    }
    return data;
}
int is_empty(queue_t *q)
{
    return q->head == NULL;
}
