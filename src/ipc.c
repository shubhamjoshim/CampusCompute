
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

struct sharedJob {
    int jobId;
    int arrival;
    int completion;
    int turnaround;
};

static int waitSemaphore(sem_t *sem) {
    while (sem_wait(sem) == -1) {
        if (errno != EINTR) {
            perror("sem_wait");
            return -1;
        }
    }
    return 0;
}

int runIPC(void) {
    char shmName[64];
    char semName[64];

    snprintf(shmName, sizeof(shmName),
             "/campus_shm_%ld", (long)getpid());

    snprintf(semName, sizeof(semName),
             "/campus_sem_%ld", (long)getpid());

    int fd = -1;
    sem_t *sem = SEM_FAILED;
    struct sharedJob *shared = MAP_FAILED;
    int result = 1;

    fd = shm_open(shmName, O_CREAT | O_EXCL | O_RDWR, 0600);

    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(fd, sizeof(struct sharedJob)) == -1) {
        perror("ftruncate");
        goto cleanup;
    }

    shared = mmap(NULL, sizeof(struct sharedJob),
                  PROT_READ | PROT_WRITE,
                  MAP_SHARED, fd, 0);

    if (shared == MAP_FAILED) {
        perror("mmap");
        goto cleanup;
    }

    if (close(fd) == -1) {
        perror("close");
        fd = -1;
        goto cleanup;
    }
    fd = -1;

    sem = sem_open(semName, O_CREAT | O_EXCL, 0600, 0);

    if (sem == SEM_FAILED) {
        perror("sem_open");
        goto cleanup;
    }

    shared->jobId = 1;
    shared->arrival = 2;
    shared->completion = 10;
    shared->turnaround = -1;

    printf("Created shared memory\n");
    printf("Job ID: %d\n", shared->jobId);
    printf("Arrival: %d\n", shared->arrival);
    printf("Completion: %d\n", shared->completion);

    fflush(stdout);

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        goto cleanup;
    }

    if (pid == 0) {
        printf("Child process: %ld\n", (long)getpid());
        printf("Reading job %d\n", shared->jobId);

        shared->turnaround =
            shared->completion - shared->arrival;

        printf("Calculated turnaround: %d\n",
               shared->turnaround);

        fflush(stdout);

        if (sem_post(sem) == -1) {
            perror("sem_post");
            _exit(1);
        }

        if (sem_close(sem) == -1) {
            perror("sem_close");
            _exit(1);
        }

        if (munmap(shared, sizeof(struct sharedJob)) == -1) {
            perror("munmap");
            _exit(1);
        }

        _exit(0);
    }

    int status;

    if (waitSemaphore(sem) == -1) {
        kill(pid, SIGTERM);
        waitpid(pid, NULL, 0);
        goto cleanup;
    }

    printf("Received result from child\n");
    printf("Job %d turnaround time: %d\n",
           shared->jobId, shared->turnaround);

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        goto cleanup;
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("Child exited successfully\n");
        result = 0;
    } else {
        printf("Child terminated unsuccessfully\n");
    }

cleanup:
    if (shared != MAP_FAILED) {
        if (munmap(shared, sizeof(struct sharedJob)) == -1) {
            perror("munmap");
            result = 1;
        }
    }

    if (sem != SEM_FAILED) {
        if (sem_close(sem) == -1) {
            perror("sem_close");
            result = 1;
        }

        if (sem_unlink(semName) == -1) {
            perror("sem_unlink");
            result = 1;
        }
    }

    if (fd != -1) {
        if (close(fd) == -1) {
            perror("close");
            result = 1;
        }
    }

    if (shm_unlink(shmName) == -1) {
        perror("shm_unlink");
        result = 1;
    }

    if (result == 0) {
        printf("IPC resources cleaned up\n");
    }

    return result;
}

int main(void) {
    return runIPC();
}

