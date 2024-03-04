#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef struct Queue
{
    int *array;
    int back;
    int front;
    int size;
} Queue;

int isEmptyQueue(Queue q)
{
    return (q.back == q.front);
}

bool areQueuesEmpty(Queue a, Queue b, Queue c)
{
    if (isEmptyQueue(a) && isEmptyQueue(b) && isEmptyQueue(c))
    {
        return true;
    }
    return false;
}

void queueEmptyError(void)
{
    printf("queue empty\n");
    abort();
}

Queue newQueue(int s)
{
    Queue q;
    q.array = malloc(s * sizeof(*q.array));
    // assert(q.array != NULL);
    q.back = 0;
    q.front = 0;
    q.size = s;
    return q;
}

// Enqueue Deuque theloun allama apo edit operation type se process. oti theloume stin periptosi mas

void enqueue(int page, Queue *qp)
{
    qp->array[qp->back] = page;
    qp->back = (qp->back + 1) % qp->size;
}

int dequeue(Queue *qp)
{
    int page;
    if (isEmptyQueue(*qp))
    {
        queueEmptyError();
    }
    page = qp->array[qp->front];
    qp->front = (qp->front + 1) % qp->size;
    return page;
}

void freeQueue(Queue q)
{
    free(q.array);
}

int main()
{
    int frames;
    Queue pagequeue = newQueue(50);
    int page;
    scanf("%d\n", &frames);
    char input;
    scanf("%c", &input);
    while (input != '\n')
    {
        if (input == ' ')
        {
            scanf("%c", &input);
            continue;
        }
        int page = input - '0';
        enqueue(page, &pagequeue);
        scanf("%c", &input);
    }
    int framearray[frames];
    for (int i = 0; i < frames; i++)
    {
        framearray[i] = -1;
    }
    int pagefaults = 0;
    int index = 0; // Where to insert the next page

    while (!isEmptyQueue(pagequeue))
    {
        int page = dequeue(&pagequeue);
        int flag = 0;
        for (int i = 0; i < frames; i++)
        {
            if (framearray[i] == page)
            {
                flag = 1;
                index = i;
                break;
            }
        }
        if (flag == 0)
        {
            framearray[index] = page;
            index = (index + 1) % frames;
            pagefaults++;
        }
    }
    printf("%d\n", pagefaults);

    return 0;
}