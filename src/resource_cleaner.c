#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>
#define TEMP_FILE "temp_worker.log"

void cleanup_resources(void) {
    printf("FR7 Cleanup: \n");
    if (unlink(TEMP_FILE) == 0) {
        printf("[Cleanup] Deleted '%s'.\n", TEMP_FILE);
    }
    printf("[Cleanup] All child processes cleared (0 zombies).\n");
}
void demo_cpu_limit(void) {
    printf("CPU Limit: \n");
    fflush(stdout);

    pid_t pid = fork();

    if (pid == 0) {
        struct rlimit rl;
        rl.rlim_cur = 1;
        rl.rlim_max = 2;
        setrlimit(RLIMIT_CPU, &rl); // caps this process to 1s of CPU time; exceeding it causes the kernel to kill the process with signal 24 (SIGXCPU)
        printf("Worker %d: CPU limit set to 1s. Starting infinite loop\n", getpid());
        fflush(stdout);

        while (1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            printf("Parent Worker %d killed by OS Signal %d (%s)\n", 
                   pid, sig, strsignal(sig));
        }
    }
}
void demo_memory_limit(void) {
    printf("Memory Limit: \n");
    fflush(stdout);
    pid_t pid = fork();
    if (pid == 0) {
        struct rlimit rl;
        rl.rlim_cur = 10 * 1024 * 1024;
        rl.rlim_max = 10 * 1024 * 1024;
        setrlimit(RLIMIT_AS, &rl);  //caps this process's virtual memory at 10 MB; any malloc() exceeding this limit is blocked by the kernel and returns NULL
        printf("Worker %d: RAM limit set to 10 MB. Trying to malloc 50 MB. \n", getpid());
        char *ptr = malloc(50 * 1024 * 1024);
        if (ptr == NULL) {
            printf("[Worker %d] malloc() failed as expected: %s\n", getpid(), strerror(errno));
            _exit(3);
        }
        free(ptr);
        _exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Parent Worker %d exited cleanly with code %d.\n", 
                   pid, WEXITSTATUS(status));
        }
    }
}
int main(void) {
    atexit(cleanup_resources);
    FILE *fp = fopen(TEMP_FILE, "w");
    if (fp) {
        fprintf(fp, "Temporary log data\n");
        fclose(fp);
        printf("[Init] Created '%s'.\n", TEMP_FILE);
    }
    demo_cpu_limit();
    demo_memory_limit();
    return 0;
}
