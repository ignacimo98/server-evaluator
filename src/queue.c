#include "queue.h"

struct Queue *create_queue(unsigned capacity)
{
    struct Queue *queue = (struct Queue *)malloc(
        sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (int *)malloc(
        queue->capacity * sizeof(int));
    return queue;
}

int is_full(struct Queue *queue)
{
    return (queue->size == queue->capacity);
}

int is_empty(struct Queue *queue)
{
    return (queue->size == 0);
}

void enqueue(struct Queue *queue, int item)
{
    if (is_full(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

int dequeue(struct Queue *queue)
{
    if (is_empty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

int front(struct Queue *queue)
{
    if (is_empty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

int rear(struct Queue *queue)
{
    if (is_empty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}