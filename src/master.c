#include <stdio.h>
#include <stdlib.h>
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
int main(void) {
    int choice;

    while (1) {
        printf("\n--- CAMPUS COMPUTE MASTER MENU---\n");
        printf("1.job management and process lifestyle \n");
        printf("2.CPU scheduling \n");
        printf("3.activity logger \n");
        printf("5.resource safety analysis \n");    
        printf("6.worker failure detection \n");    
        printf("7.resource limit and cleanup \n");    
        printf("0. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1 || choice == 0) {
            printf("Exiting CampusCompute\n");
            break;
        }

        switch (choice) {
            case 1:
            int id=0;
            int arrival=0;
            int burst=0;
            int priority=0;
            char option;
            while(1){
            printf("\n choose what you want to execute: \n");
            printf("   a.add job \n");
            printf("   b.display jobs \n");
            printf("   c.reset jobs \n ");
            printf("   0.exit \n");
            scanf(" %c",&option);
            
            
            switch(option){
                case 'a':
                case 'A':
                printf("adding a job, give me job id,arrival time,burst time and priority in that order:\n ");
                scanf("%d %d %d %d",&id,&arrival,&burst,&priority);
                addJob(id,arrival,burst,priority);
                break;
                case 'b':
                case 'B':
                printf("displaying the jobs listed: \n");
                displayJobs();
                break;
                case 'c':
                case 'C':
                printf("resetting jobs\n");
                resetJobs();
                printf("jobs have been reset");
                break;
                case '0':break;
                default: 
                printf("invalid input , enter a valid input");
                break;
            }
            }


            case 2:
            char opt;
            int quantum;
             while (1) {
            printf("a. FCFS\n");
            printf("b. Round Robin\n");
            printf("c. Priority Scheduling\n");
            printf("x. Back to main menu\n");
            printf("Enter option: ");
            if (scanf(" %c", &opt) != 1) break;
            if (opt == 'x' || opt == 'X')
            {
                break;
            }


            switch (opt) {
            case 'a':
            case 'A':
                fcfs();
                displayJobs();
                break;

            case 'b':
            case 'B':
                printf("Enter time quantum: ");
                if (scanf("%d", &quantum) != 1 || quantum <= 0) {
                    printf("Invalid quantum, enter a positive integer.\n");
                    while (getchar() != '\n');        /* clear bad input */
                    break;
                }
                roundRobin(quantum);
                displayJobs();
                break;

            case 'c':
            case 'C':
                priorityScheduling();
                displayJobs();
                break;


            default:
                printf("Invalid option, enter a, b, c or x.\n");
                break;
        }
    }
    break;

            case 3:
            runLogger();
            break;

            case 4:
            runIPC();
            break;

            case 5:{
            int flag;
            printf("---BANKER'S ALGORITHM---\n");
            printf("1. success case\n");
            printf("2. failure case\n");
            printf("0. exit \n");
            if (scanf("%d", &flag) != 1 || flag == 0) {
            printf("invalid input\n");
            break;
            }
            
            switch(flag){
                case 1:
                banker_algo_success();
                break;
                case 2:
                banker_algo_deadlock();
                break;
                case 0: break;
            }
            

            case 6:
            printf("worker error detection: \n");
            int n;
            printf("1. success case\n");
            printf("2. failure case\n");
            printf("0.exit \n);

            if (scanf("%d", &n) != 1 || n == 0) {
            printf("invalid input\n");
            break;
             switch (n){   
             case 1:
                success();
                break;
                case 2:
                failure();
                break;
                 case 0: break;
             }

             case 7:
             atexit(cleanup_resources);
            FILE *fp = fopen(TEMP_FILE, "w");
            if (fp) {
            fprintf(fp, "Temporary log data\n");
            fclose(fp);
            printf("[Init] Created '%s'.\n", TEMP_FILE);
            }
            demo_cpu_limit();
            demo_memory_limit();
            break;

            default:
                printf("Invalid choice! Pick 0-7 only.\n");
        }
    }

    return 0;
}
    }
}
