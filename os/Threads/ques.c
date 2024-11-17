#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>  // Ensure this is included for 'wait()'

#define NUM_PROCESSES 10000
#define NUM_THREADS 10000

void* thread_func(void* arg) {
    return NULL;
}

void measure_fork_time() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (fork() == 0) {
            exit(0);  // Child process exits immediately
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);  // Parent waits for all child processes
    }

    gettimeofday(&end, NULL);

    double real_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    double user_time = usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1000000.0;
    double sys_time = usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1000000.0;

    printf("Fork() - Real Time: %.6f seconds, User Time: %.6f seconds, Sys Time: %.6f seconds\n", real_time, user_time, sys_time);
}

void measure_thread_time() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, NULL) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);

    double real_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    double user_time = usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1000000.0;
    double sys_time = usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1000000.0;

    printf("pthread_create() - Real Time: %.6f seconds, User Time: %.6f seconds, Sys Time: %.6f seconds\n", real_time, user_time, sys_time);
}

int main() {
    printf("Measuring time for forking 10,000 processes and creating 10,000 threads:\n");

    printf("\nMeasuring Fork() time...\n");
    measure_fork_time();

    printf("\nMeasuring pthread_create() time...\n");
    measure_thread_time();

    return 0;
}

