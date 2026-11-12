#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define READY 0 
#define RUNNING 1
#define COMPLETED 2

struct job{
  int id;
  int arrival;
  int burst;
  int priority;
  int remaining;
  int completion;
  int status;

  int totalTime;
  int waitTime;
};

struct node{
  int index;
  struct node *next;
};

struct node *head = NULL;
struct node *tail = NULL;
struct job jobs[100];
int jobCount = 0;


int dequeue(struct node **head, struct node **tail){
  if((*head) == NULL) return -1;
  
  struct node *temp = (*head);
  (*head) = (*head) -> next;
  if((*head) == NULL) (*tail) = NULL;
  int index = temp -> index;
  free(temp);
  return index;
} 

void enqueue(int index,struct node **head, struct node **tail){
  if(index < 0 || index >=jobCount){
    printf("Incorrect Index - enqueue\n");
    return;
  }
 
  struct node *new = malloc(sizeof(struct node));
  if(new == NULL){
    printf("Malloc failed\n");
    return;
  }
  new -> index = index;
  new -> next = NULL;
  if((*head) == NULL){
    (*head) = new; 
    (*tail) = new;
    return;
  }

  (*tail) -> next = new;
  (*tail) = (*tail) -> next;
  return;
}

void addJob(int id,int arrival,int burst,int priority){
  if(jobCount == 100){
    printf("Cannot add job - job queue full\n");
    return;
  }
  if(arrival < 0) {printf("Incorrect arrival time\n"); return;}
  if(burst <= 0){
    printf("Incorrect burst time\n");
    return;
  }

  for(int i = 0;i<jobCount;i++){
    if(jobs[i].id == id){
      printf("Job already exists\n");
      return;
    }
  }

  struct job new;
  new.id = id;
  new.arrival = arrival;
  new.burst = burst;
  new.priority = priority;
  new.remaining = burst;
  new.completion = 0;
  new.status = READY;
  new.waitTime = -1;
  new.totalTime = -1;

  jobs[jobCount] = new;
  jobCount++;

  return;
}

void displayJobs(){
  if(jobCount == 0){
    printf("No jobs to display :)\n");
    return;
  }
  printf("%-10s %-10s %-10s %-12s %-10s %-10s %-10s\n",
       "JOB", "Arrival", "Burst", "Remaining", "Status","TAT","WT");

for (int i = 0; i < jobCount; i++) {
    printf("%-10d %-10d %-10d %-12d %-10d %-10d %-10d\n",
           jobs[i].id,
           jobs[i].arrival,
           jobs[i].burst,
           jobs[i].remaining,
           jobs[i].status,
           jobs[i].totalTime,
           jobs[i].waitTime);
}

}

void resetJobs(){
  if(jobCount == 0){
    printf("No jobs to reset :p\n");
    return;
  }

  for(int i=0;i<jobCount;i++){
    jobs[i].remaining = jobs[i].burst;
    jobs[i].status = READY;
    jobs[i].completion = 0;
  }

  printf("Jobs reset successfully\n");
}

void sort(int arr[100]){
  for(int i=0;i<jobCount;i++){
    for(int j=i;j<jobCount;j++){
      if(jobs[arr[i]].arrival > jobs[arr[j]].arrival){
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
    }
  }
}

void execute(int *clock,int duration, int index){
  if(duration <= 0 || duration > jobs[index].remaining){
      printf("Incorrect duration X(\n");
      return;
  }

  if(index <0 || index >= jobCount){
    printf("Wrong index\n");
    return;
  }
  if(jobs[index].arrival > *clock){
    printf("Job has not arrived yet\n");
    return;
  }
  if(jobs[index].status != READY) return;
  jobs[index].status = RUNNING;
  (*clock) += duration;
  jobs[index].remaining -= duration;
  
  if(jobs[index].remaining == 0){
    jobs[index].status = COMPLETED;
    jobs[index].completion = *clock;
    return;
  }
  jobs[index].status = READY;  
}

void calculateTime(){
  if(jobCount == 0){
    printf("No jobs\n");
    return;
  }
  int totalSum = 0;
  int waitSum = 0;
  for(int i=0;i<jobCount;i++){
    jobs[i].totalTime = jobs[i].completion - jobs[i].arrival;
    totalSum += jobs[i].totalTime;
    jobs[i].waitTime = jobs[i].totalTime - jobs[i].burst;
    waitSum += jobs[i].waitTime;
  }
  return;
}

void fcfs(void){
  
  if(jobCount == 0){
    printf("NO JOBS :p\n");
    return;
  }

  resetJobs();

  int order[100];
  
  for(int i=0;i<jobCount;i++){
    order[i] = i;
  }
  sort(order);

  int clock = 0;

  for(int i=0;i<jobCount;i++){
    int index = order[i];

    if(clock < jobs[index].arrival) clock = jobs[index].arrival;

    execute(&clock,jobs[index].remaining,index);
  }
  calculateTime();
}

void roundRobin(int quantum){
  
  if(quantum <= 0){
    printf("Incorrect quantum\n");
    return;
  }

  if(jobCount == 0){
    printf("No jobs available\n");
    return;
  }

  resetJobs();

  int clock = 0;
  int completed = 0;
  int nextArrival = 0;

  struct node *rrHead = NULL;
  struct node *rrTail = NULL;

  int order[100];

  for(int i=0;i<jobCount;i++){
    order[i] = i;
  }

  sort(order);
while(completed < jobCount){
  while(nextArrival < jobCount && jobs[order[nextArrival]].arrival <=clock){

    enqueue(order[nextArrival],&rrHead,&rrTail);
    nextArrival++;

  }
    if(rrHead == NULL){
      if(nextArrival < jobCount)
        clock = jobs[order[nextArrival]].arrival;
      continue;
    }

    int index = dequeue(&rrHead,&rrTail);

    int duration = quantum;

    if(jobs[index].remaining < quantum){
      duration = jobs[index].remaining;
    }

    execute(&clock,duration, index);

    while(nextArrival < jobCount && jobs[order[nextArrival]].arrival <= clock){
      enqueue(order[nextArrival],&rrHead,&rrTail);
      nextArrival++;
    }

    if(jobs[index].remaining == 0){
      completed ++;
    }
    else{
      enqueue(index,&rrHead,&rrTail);
    }
  
}
  calculateTime();

}

int main(){

  int clock = 0;

  addJob(1,1,1,1);
  addJob(2,2,3,5);
  addJob(5,0,1,1);
  addJob(6,0,5,1);
  addJob(9,2,3,5);
  displayJobs();
  fcfs();
  displayJobs();
  roundRobin(2);
  displayJobs();
}
