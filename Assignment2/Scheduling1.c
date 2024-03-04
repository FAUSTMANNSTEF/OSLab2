/* based on queueTypeAndFunctions.c by Gerard Renardel, 31 January 2018 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef struct Process
{
    unsigned long id; // Process ID/Arrival time in this case
    int *array;       // Time for the process to be executed
    unsigned long pointer;
    unsigned long timeRunning;
    unsigned long size; // Size of the array
} Process;

typedef struct Queue
{
    Process *array;
    unsigned long back;
    unsigned long front;
    unsigned long size;
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

Queue newQueue(unsigned long s)
{
    Queue q;
    q.array = malloc(s * sizeof(*q.array));
    // assert(q.array != NULL);
    q.back = 0;
    q.front = 0;
    q.size = s;
    return q;
}

void queueEmptyError(void)
{
    printf("queue empty\n");
    abort();
}

void doubleQueueSize(Queue *qp)
{
    unsigned long i;
    unsigned long oldSize = qp->size;
    qp->size = 2 * oldSize;
    qp->array = realloc(qp->array, qp->size * sizeof(*qp->array));
    assert(qp->array != NULL);
    for (i = 0; i < qp->back; i++)
    { /* eliminate the split configuration */
        qp->array[oldSize + i] = qp->array[i];
    }
    qp->back = qp->back + oldSize; /* update qp->back */
}

// Enqueue Deuque theloun allama apo edit operation type se process. oti theloume stin periptosi mas

void enqueue(Process item, Queue *qp)
{
    qp->array[qp->back] = item;
    qp->back = (qp->back + 1) % qp->size;
    if (qp->back == qp->front)
    {
        doubleQueueSize(qp);
    }
}

Process dequeue(Queue *qp)
{
    Process item;
    if (isEmptyQueue(*qp))
    {
        queueEmptyError();
    }
    item = qp->array[qp->front];
    qp->front = (qp->front + 1) % qp->size;
    return item;
}

void freeQueue(Queue q)
{
    free(q.array);
}

// Printing function used for debugging
void printResult(Queue initial, int actualqueuesize)
{
    printf("Amount of processes read is %d \n", actualqueuesize);
    Queue some = initial;
    for (int k = 0; k < actualqueuesize; k++)
    {
        Process temp = dequeue(&some);

        printf("The temp size is %d \n", temp.size);
        for (int l = 0; l < temp.size; l++)
        {
            printf("%d ", temp.array[l]);
        }
        printf("\n");
    }
}

int main()
{
    unsigned long j = 0;               // process array elements
    int flag = 0;                      // flag for EOF
    int c = 0;                         // input number
    unsigned long actualqueuesize = 0; // processes read

    // Reads initial input inorder to dynamically allocate it after
    Queue initial = newQueue(50);
    while (1)
    {
        Process input;
        input.array = (int *)malloc(50 * sizeof(int));
        j = 0;
        int processaraysize = 50;
        while (1)
        {
            if (j > processaraysize)
            {
                input.array = (int *)realloc(input.array, processaraysize * 2 * sizeof(int));
                processaraysize = processaraysize * 2;
            }

            if (scanf("%d", &c) == EOF)
            {
                flag = 1;
                break;
            }
            if (j == 0)
            {
                input.id = c;
            }
            input.array[j] = c;
            j++;
            if (c == (-1))
            {
                input.size = j;
                input.pointer = 0;
                input.timeRunning = 0;
                break;
            }
        }
        if (flag == 1)
        {
            break;
        }
        if (actualqueuesize > initial.size)
        {
            doubleQueueSize(&initial);
        }

        enqueue(input, &initial);
        actualqueuesize++;
    }

    // printResult(initial, actualqueuesize);

    // Transfers to queue of exact size and exact process array size for each process

    Queue ready = newQueue(actualqueuesize);
    for (unsigned long i = 0; i < actualqueuesize; i++)
    {
        Process temp = dequeue(&initial);
        enqueue(temp, &ready);
    }
    freeQueue(initial);

    // INITIALIZING VARIABLES
    float totalTime = 0.0;
    unsigned long tick = 0;
    Queue CPU = newQueue(actualqueuesize);
    Queue IO = newQueue(actualqueuesize);

    Process workingCPU;
    Process workingIO;

    bool isCPUWorking = false;
    bool isIOWorking = false;

    unsigned long workingCPUTime = 0;
    unsigned long workingIOTime = 0;

    unsigned long itemsWitingForCPU = 0;
    unsigned long itemsWitingForIO = 0;

    // The main loop of the program and the tick counter
    while (!(areQueuesEmpty(CPU, IO, ready)) || isCPUWorking || isIOWorking)
    {

        // check if the process has arrived
        if (!isEmptyQueue(ready) && ready.array[ready.front].array[0] == tick)
        {
            Process temp = dequeue(&ready);
            temp.pointer++;
            enqueue(temp, &CPU);
            itemsWitingForCPU++;
        }
        if (isCPUWorking)
        {
            workingCPU.timeRunning++;
            workingCPUTime++;
        }
        if (isIOWorking)
        {
            workingIO.timeRunning++;
            workingIOTime++;
        }

        if (!isCPUWorking)
        {
            if (!isEmptyQueue(CPU))
            {
                workingCPU = dequeue(&CPU);
                itemsWitingForCPU--;
                workingCPUTime = 0;
                isCPUWorking = true;
            }
        }
        // check if the IO is working, if not get a new process
        if (!isIOWorking)
        {
            if (!isEmptyQueue(IO))
            {
                workingIO = dequeue(&IO);
                itemsWitingForIO--;
                workingIOTime = 0;
                isIOWorking = true;
            }
        }
        // check if the CPU is done working and if so, put it in the IO queue if not increment the time
        if (isCPUWorking)
        {
            if (workingCPUTime == workingCPU.array[workingCPU.pointer])
            {
                workingCPU.pointer++;
                isCPUWorking = false;
                workingCPUTime = 0;
                if (workingCPU.array[workingCPU.pointer] != -1)
                {
                    enqueue(workingCPU, &IO);
                    itemsWitingForIO++;
                }
                else
                {
                    totalTime += workingCPU.timeRunning;
                }
            }
        }
        // check if the IO is done working and if so, put it in the ready queue if not increment the time
        if (isIOWorking)
        {
            if (workingIOTime == workingIO.array[workingIO.pointer])
            {
                workingIO.pointer++;
                isIOWorking = false;
                workingIOTime = 0;
                if (workingIO.array[workingIO.pointer] != -1)
                {
                    enqueue(workingIO, &CPU);
                    itemsWitingForCPU++;
                }
                else
                {
                    totalTime += workingIO.timeRunning;
                }
            }
        }
        if (!isCPUWorking)
        {
            if (!isEmptyQueue(CPU))
            {
                workingCPU = dequeue(&CPU);
                itemsWitingForCPU--;
                workingCPUTime = 0;
                isCPUWorking = true;
            }
        }
        // check if the IO is working, if not get a new process
        if (!isIOWorking)
        {
            if (!isEmptyQueue(IO))
            {
                workingIO = dequeue(&IO);
                itemsWitingForIO--;
                workingIOTime = 0;
                isIOWorking = true;
            }
        }
        totalTime += itemsWitingForCPU + itemsWitingForIO;
        tick++;

        // printf("Tick : %d and totalTime %d\n", tick, totalTime);
        // if (isCPUWorking)
        // {
        //   printf("Working CPU Time: %d and is CPU wokring %d\n", workingCPUTime, isCPUWorking);
        //   printf("---------------Process working CPU--------------\n");
        //   printf("id:%d\n", workingCPU.id);
        //   printf("size:%d\n", workingCPU.size);
        //   for (int k = 0; k < workingCPU.size; k++)
        //   {
        //     printf("::%d\n", workingCPU.array[k]);
        //   }
        //   printf("pointer:%d\n", workingCPU.pointer);
        //   printf("time:%d\n", workingCPU.timeRunning);
        //   printf("----------------------------------------\n");
        // }

        // if (isIOWorking)
        // {
        //   printf("Working IO Time: %d and is IO wokring %d\n", workingIOTime, isIOWorking);
        //   printf("---------------Process working IO--------------\n");
        //   printf("id:%d\n", workingIO.id);
        //   printf("size:%d\n", workingIO.size);
        //   for (int k = 0; k < workingIO.size; k++)
        //   {
        //     printf("::%d\n", workingIO.array[k]);
        //   }
        //   printf("pointer:%d\n", workingIO.pointer);
        //   printf("time:%d\n", workingIO.timeRunning);
        //   printf("----------------------------------------\n");
        // }
    }

    if (actualqueuesize == 0)
    {
        printf("0\n");
        return 0;
    }
    // printf("%d\n", tick);
    printf("%ld\n", (unsigned long)totalTime / actualqueuesize);
    freeQueue(CPU);
    freeQueue(IO);
    freeQueue(ready);

    return 0;
}
