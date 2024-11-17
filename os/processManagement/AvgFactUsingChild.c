#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

// Function to compute factorial
long long factorial(int n) {
    long long fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

// Function to compute average
double compute_average(int n) {
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i; // For simplicity, adding numbers 1 to n
    }
    return (double)sum / n;
}

int main() {
    pid_t pid;
    int n = 10;  // Number of elements for average calculation
    int fact_num = 9;  // Number for factorial computation

    // Timer to measure execution time
    clock_t start_time, end_time;
    double child_time, parent_time;

    // Start time for both parent and child processes
    start_time = clock();

    // Create a new child process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process to compute the average of 'n' numbers
        printf("Child process (PID: %d) calculating the average of numbers from 1 to %d\n", getpid(), n);
        double avg = compute_average(n);
        printf("Child process: The average of numbers from 1 to %d is %.2f\n", n, avg);

        // Measure execution time for child
        end_time = clock();
        child_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Child process execution time: %.4f seconds\n", child_time);

        exit(0);  // Exit child process
    } else {
        // Parent process: compute the factorial of a given number
        printf("Parent process (PID: %d) calculating the factorial of %d\n", getpid(), fact_num);

        long long fact = factorial(fact_num);
        printf("Parent process: The factorial of %d is %lld\n", fact_num, fact);

        // Wait for the child to complete
        wait(NULL);

        // Measure execution time for parent
        end_time = clock();
        parent_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Parent process execution time: %.4f seconds\n", parent_time);
    }

    return 0;
}

