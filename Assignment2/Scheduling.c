/* based on queueTypeAndFunctions.c by Gerard Renardel, 31 January 2018 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef struct Process {
int id ; //Process ID/Arrival time in this case
int array[1000] ; //Time for the process to be executed
int pointer;
int timeRunning;
int size ;//Size of the array
} Process;

typedef struct Queue {
  Process *array;
  int back;
  int front;
  int size;
} Queue;

//Function that adds one to a processes timeRunning for all the processes in the queue
void addOneToTimeRunning(Queue *qp){
  for(int i=0; i<qp->size; i++){
    if(qp==NULL){
      break;
    }
    if(qp->array[i].array[qp->array[i].pointer]!=-1){
      qp->array[i].timeRunning++;
      printf("%d timeRunning: %d\n",qp->array[i].id , qp->array[i].timeRunning);
    }    
  }
}



int isEmptyQueue(Queue q) {
  return (q.back == q.front);
}

bool areQueuesEmpty(Queue a, Queue b, Queue c){
  if(isEmptyQueue(a) && isEmptyQueue(b) && isEmptyQueue(c)){
    return true;
  }
  return false;
}

Queue newQueue(int s) {
  Queue q;
  q.array = malloc(s*sizeof(*q.array));
  assert(q.array != NULL);
  q.back = 0;
  q.front = 0;
  q.size = s;
  return q;
}



void queueEmptyError(void) {
  printf("queue empty\n");
  abort();
}

void doubleQueueSize(Queue *qp) {
  int i;
  int oldSize = qp->size;
  qp->size = 2 * oldSize;
  qp->array = realloc(qp->array, qp->size * sizeof(*qp->array));
  assert(qp->array != NULL);
  for (i=0; i < qp->back ; i++) { /* eliminate the split configuration */
    qp->array[oldSize + i] = qp->array[i];
  }
  qp->back = qp->back + oldSize;  /* update qp->back */
}

//Enqueue Deuque theloun allama apo edit operation type se process. oti theloume stin periptosi mas

void enqueue (Process item, Queue *qp) {
  qp->array[qp->back] = item;
  qp->back = (qp->back + 1) % qp->size;
  if ( qp->back == qp->front ) {
    doubleQueueSize(qp);
  }
}

Process dequeue (Queue *qp) {
  Process item;
  if (isEmptyQueue(*qp)) {
    queueEmptyError();
  }
  item = qp->array[qp->front];
  qp->front = (qp->front + 1) % qp->size;
  return item;
}

void freeQueue (Queue q) {
  free(q.array);
}


int main(){
    int j=0;
  int flag = 0;
  int c=0;
  int i = 0;
  int totalTime=0;

  Queue ready = newQueue(1000);
  while (1) {
    Process input;
    j=0;
    while (1){
      if(scanf("%d", &c)==EOF){
        flag=1;
        break;
      }
      if(j==0){
        input.id = c;
      }
      input.array[j]= c;
      j++;
      if(c==(-1)){
        input.size = j;
        input.pointer = 0;
        input.timeRunning = 0;
        break;
      }
      
    }
    if(flag==1){
      break;
    }
    
    enqueue(input, &ready);
    i++;
  }

  Queue some = ready; 
    for(int k=0; k<i; k++){
    Process temp = dequeue(&some);
    for(int l=0; l<temp.size; l++){
      printf("%d ", temp.array[l]);
    }
    printf("\n");
  }

  int tick=0;
  Queue CPU = newQueue(1000);
  Queue IO = newQueue(1000); 

  Process workingCPU;
  Process workingIO;

  bool isCPUWorking = false;
  bool isIOWorking = false;

  int workingCPUTime = 0;
  int workingIOTime = 0;
  int waitingTime=0;
  
  //The main loop of the program and the tick counter
  while(!areQueuesEmpty(CPU, IO, ready) || isCPUWorking || isIOWorking){

    //check if the process has arrived
    if(!isEmptyQueue(ready) && ready.array[ready.front].array[0]==tick){
      Process temp = dequeue(&ready);
      temp.pointer++;
      enqueue(temp, &CPU);
    }
    //check if the CPU is done working and if so, put it in the IO queue if not increment the time
    if(isCPUWorking){
      if(workingCPUTime==workingCPU.array[workingCPU.pointer]){
        isCPUWorking = false;
        if(workingCPU.array[workingCPU.pointer+1]!=-1){
          workingCPU.pointer++;
          enqueue(workingCPU, &IO);
        }
        else{
          printf("workingCPU.timeRunning: %d\n", workingCPU.timeRunning);
          totalTime+=workingCPU.timeRunning;
        }
      }
      else{
        workingCPUTime++;
      }
    }
    //check if the IO is done working and if so, put it in the ready queue if not increment the time
    if(isIOWorking){
      if(workingIOTime==workingIO.array[workingIO.pointer]){
        isIOWorking = false;
        if(workingIO.array[workingIO.pointer+1]!=-1){
          workingIO.pointer++;
          enqueue(workingIO, &CPU);
        }
        else{
          printf("workingIO.timeRunning: %d\n", workingIO.timeRunning);
          totalTime+=workingIO.timeRunning;
        }
      }
      else{
        workingIOTime++;
      }
    }
    //check if the CPU is working, if not get a new process
    if(!isCPUWorking){
      if(!isEmptyQueue(CPU)){
        workingCPU = dequeue(&CPU);
        workingCPUTime=0;
        isCPUWorking = true;
      }
    }
    //check if the IO is working, if not get a new process
    if(!isIOWorking){
      if(!isEmptyQueue(IO)){
        workingIO = dequeue(&IO);
        workingIOTime=0;
        isIOWorking = true;
      }
    }
    addOneToTimeRunning(&CPU);
    addOneToTimeRunning(&IO);
    if(isCPUWorking){
      workingCPU.timeRunning++;
      printf("%d timeRunning: %d\n",workingCPU.id , workingCPU.timeRunning);
    }
    if(isIOWorking){
      workingIO.timeRunning++;
      printf("%d timeRunning: %d\n",workingIO.id , workingIO.timeRunning);
    }
    
    tick++;
    

  }

  printf("%d\n", tick);
  printf("%d\n", totalTime/i);

  return 0;
}
