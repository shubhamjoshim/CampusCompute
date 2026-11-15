#include <math.h>
#include <stdio.h>
#include <wchar.h>
#include "stdlib.h"
#define TEMP_FILE "temp_worker.log"


void addJob(int id, int arrival, int burst, int priority);
void displayJobs(void);
void resetJobs(void);
void fcfs(void);
void roundRobin(int quantum);
void priorityScheduling(void);
int runLogger(void);
int runIPC(void);
void banker_algo_success(void);
void banker_algo_deadlock(void);
int success(void);
int failure(void);
void cleanup_resources(void);
void demo_cpu_limit(void);
void demo_memory_limit(void);


int main(){
  int masterChoice;

  while(1){
    printf("MASTER MENU\n");
    printf("1.job management and process lifestyle \n");
    printf("2.CPU scheduling \n");
    printf("3.activity logger \n");
    printf("5.resource safety analysis \n");    
    printf("6.worker failure detection \n");    
    printf("7.resource limit and cleanup \n");    
    printf("0. Exit\n");
    printf("Enter choice: ");
    scanf("%d",&masterChoice);

    if(masterChoice == 0){
      printf("Exiting\n");
      return 0;
    }

    if(masterChoice == 1){
      int id;
      int arrival;
      int burst;
      int priority;
      int option;

      int running = 1;

      while(running){
          printf("\n choose what you want to execute: \n");
          printf("1.add job \n");
          printf("2.display jobs \n");
          printf("3.reset jobs \n");
          printf("0.exit \n");

          scanf("%d",&option);

          if(option == 1){
            printf("Enter jobID, AT, BT, Priority: ");
            scanf("%d %d %d %d",&id,&arrival,&burst,&priority);
            addJob(id,arrival,burst,priority);
          }

          else if(option == 2){
            displayJobs();
          }

          else if(option == 3){
            resetJobs();
          }

          else if(option == 0){
            running = 0;
          }

          else{
            printf("Invalid option\n");
          }
      }
    }

    else if(masterChoice == 2){
      int option;
      int quantum;

      int running = 1;

      while(running){
          printf("1. FCFS\n");
          printf("2. Round Robin\n");
          printf("3. Priority Scheduling\n");
          printf("0. Back to main menu\n");
          printf("Enter option: ");

          scanf("%d",&option);

          if(option == 1){
            fcfs();
            displayJobs();
          }

          else if(option == 2){
            printf("Enter Quantum: ");
            scanf("%d",&quantum);
            roundRobin(quantum);
            displayJobs();
          }

          else if(option == 3){
            priorityScheduling();
            displayJobs();
          }

          else if(option == 0){
            running = 0;
          }

          else{
            printf("Invalid input\n");
          }
      }
    }

    else if(masterChoice == 3){
      runLogger();
    }

    else if(masterChoice == 4){
      runIPC();
    }

    else if(masterChoice == 5){
      int option;
      printf("---BANKER'S ALGORITHM---\n");
      printf("1. success case\n");
      printf("2. failure case\n");
      printf("0. exit \n");

      scanf("%d",&option);

      if(option == 1){
        banker_algo_success();
      }

      else if(option == 2){
        banker_algo_deadlock();
      }

      else if(option == 0){

      }

      else{
        printf("Invalid input\n");
      }

    }

    else if(masterChoice == 6){
      printf("worker error detection:\n");

      int n;

      printf("1. Success Case\n");
      printf("2. failure Case\n");
      printf("0. Exit\n");

      scanf("%d",&n);

      if(n == 1){
        success();
      }
      else if(n == 2){
        failure();
      }
      else if(n == 0){

      }
      else {
        printf("Invalid input\n");
      }
    }

    else if(masterChoice == 7){
      atexit(cleanup_resources);
      FILE *fp = fopen(TEMP_FILE, "w");

      if(fp){
        fprintf(fp,"Temporary log data\n");
        fclose(fp);
        printf("Init: created '%s'.\n",TEMP_FILE);
      }
      demo_cpu_limit();
      demo_memory_limit();
    }

    else{
      printf("Invalid choice\nPick 0-7 only\n");
    }
    
  }
}
