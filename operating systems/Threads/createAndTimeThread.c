#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Thread function that does nothing for now
void* thread_func(void* arg) {
    // Just return immediately, can be used for measurement
    return NULL;
}

void create_threads_and_measure_time(int num_threads) {
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL) {
        perror("malloc");
        exit(1);
    }

    // Get the start time
    clock_t start_time = clock();

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, NULL) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
    }

    // Get the end time
    clock_t end_time = clock();

    // Calculate time taken
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Time taken for %d threads: %f seconds\n", num_threads, time_taken);

    // Free memory
    free(threads);
}

int main() {
    printf("Starting thread creation and timing...\n");

    // Measure time for 5k, 10k, and 20k threads
    create_threads_and_measure_time(5000);
    create_threads_and_measure_time(10000);
    create_threads_and_measure_time(20000);

    return 0;
}

