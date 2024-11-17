#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sched.h>

#define NUM_PROCESSES 50000

// Function to be called by the child created by clone()
int child_function(void *arg) {
    return 0; // Child does nothing but exit
}

// Create 50,000 processes using vfork()
void create_processes_vfork() {
    pid_t pid;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pid = vfork();
        if (pid < 0) {
            perror("vfork failed");
            exit(1);
        } else if (pid == 0) {
            // Child process, do nothing and exit
            _exit(0);
        } else {
            // Parent process, wait for the child
            wait(NULL);
        }
    }
}

// Create 50,000 processes using clone()
void create_processes_clone() {
    pid_t pid;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        // Create child process using clone()
        pid = clone(child_function, malloc(16384) + 8192, SIGCHLD, NULL);
        if (pid == -1) {
            perror("clone failed");
            exit(1);
        }
    }
}

int main() {
    clock_t start, end;
    double vfork_time, clone_time;

    // Measure time for vfork()
    start = clock();
    create_processes_vfork();
    end = clock();
    vfork_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Measure time for clone()
    start = clock();
    create_processes_clone();
    end = clock();
    clone_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Time taken using vfork() to create %d processes: %.4f seconds\n", NUM_PROCESSES, vfork_time);
    printf("Time taken using clone() to create %d processes: %.4f seconds\n", NUM_PROCESSES, clone_time);

    return 0;
}

