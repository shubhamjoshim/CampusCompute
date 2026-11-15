#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int failure(){
int pid=fork();
int n;
int status;

if(pid<0){
    printf("fork failed");
    return 1;
}
if (pid == 0) {
        for (int i = 1; i < 6; i++) {
            printf("%d ", i);
        }
        printf("\n");
        exit(2); 
    } else {
        int status;
        waitpid(pid, &status, 0);

        for (int i = 6; i < 11; i++) {
            printf("%d ", i);
        }
        printf("\n");


if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            if (code == 0) {
                printf("Child process exited correctly (code %d)\n", code);
            } else {
                printf(" failure detected (exit code %d)\n", code);
            }
        }
        return 0;
    }
}

int success(){
int pid=fork();
int n;
int status;

if(pid<0){
    printf("fork failed");
    return 1;
}
if (pid == 0) {
        for (int i = 1; i < 6; i++) {
            printf("%d ", i);
        }
        printf("\n");
        exit(0); 
    } else {
        int status;
        waitpid(pid, &status, 0);

        for (int i = 6; i < 11; i++) {
            printf("%d ", i);
        }
        printf("\n");


if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            if (code == 0) {
                printf("Child process exited correctly (code %d)\n", code);
            } else {
                printf(" failure detected (exit code %d)\n", code);
            }
        }
        return 0;
    }
}

int main(int argc,char*argv[] ){
    int n;
    printf("|-----FR6-----| \n");
    printf("choose which case you want to run: \n");
    printf("1. success \n");
    printf("2. failure \n");
    scanf("%d",&n);
    switch(n){
        case 1: 
            success();
            break;
        case 2: 
            failure();
            break;
        default:
            printf("give a valid input please. terminating... \n");
            return 1;
    }
}



