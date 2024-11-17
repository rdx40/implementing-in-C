#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int start;
    int end;
    int sum;
} ThreadData;

void* sum_range(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sum = 0;
    for (int i = data->start; i <= data->end; i++) {
        sum += i;
    }
    data->sum = sum;
    return NULL;
}

int main() {
    int n;
    printf("Enter a number n: ");
    scanf("%d", &n);

    ThreadData data1 = {1, n / 2, 0};
    ThreadData data2 = {n / 2 + 1, n, 0};

    pthread_t thread1, thread2;

    if (pthread_create(&thread1, NULL, sum_range, (void*)&data1) != 0) {
        perror("pthread_create for thread1");
        exit(1);
    }

    if (pthread_create(&thread2, NULL, sum_range, (void*)&data2) != 0) {
        perror("pthread_create for thread2");
        exit(1);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    int total_sum = data1.sum + data2.sum;
    printf("The summation of numbers from 1 to %d is: %d\n", n, total_sum);

    return 0;
}

