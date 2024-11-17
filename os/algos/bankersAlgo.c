#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3

void calculateNeed(int need[MAX_PROCESSES][MAX_RESOURCES], int max[MAX_PROCESSES][MAX_RESOURCES], int allocation[MAX_PROCESSES][MAX_RESOURCES]) {
    for (int i = 0; i < MAX_PROCESSES; i++)
        for (int j = 0; j < MAX_RESOURCES; j++)
            need[i][j] = max[i][j] - allocation[i][j];
}

void printMatrices(int max[MAX_PROCESSES][MAX_RESOURCES], int allot[MAX_PROCESSES][MAX_RESOURCES], int avail[]) {
    int need[MAX_PROCESSES][MAX_RESOURCES];
    calculateNeed(need, max, allot);

    printf("\nMaximum Resources Matrix:\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < MAX_RESOURCES; j++)
            printf("%d ", max[i][j]);
        printf("\n");
    }

    printf("\nAllocated Resources Matrix:\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < MAX_RESOURCES; j++)
            printf("%d ", allot[i][j]);
        printf("\n");
    }

    printf("\nNeed Matrix:\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < MAX_RESOURCES; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    printf("\nAvailable Resources: ");
    for (int i = 0; i < MAX_RESOURCES; i++)
        printf("%d ", avail[i]);
    printf("\n");
}

bool isSafe(int processes[], int numProcesses, int avail[], int max[][MAX_RESOURCES], int allot[][MAX_RESOURCES]) {
    int need[MAX_PROCESSES][MAX_RESOURCES];
    calculateNeed(need, max, allot);

    bool finish[MAX_PROCESSES] = {0};
    int safeSeq[MAX_PROCESSES];
    int work[MAX_RESOURCES];

    for (int i = 0; i < MAX_RESOURCES; i++)
        work[i] = avail[i];

    int count = 0;
    while (count < numProcesses) {
        bool found = false;
        for (int p = 0; p < numProcesses; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < MAX_RESOURCES; j++)
                    if (need[p][j] > work[j])
                        break;

                if (j == MAX_RESOURCES) {
                    for (int k = 0; k < MAX_RESOURCES; k++)
                        work[k] += allot[p][k];

                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found)
            return false;
    }

    printf("System is in a safe state.\nSafe sequence is: ");
    for (int i = 0; i < numProcesses; i++)
        printf("%d ", safeSeq[i]);
    printf("\n");

    return true;
}

bool requestResources(int processes[], int numProcesses, int avail[], int max[][MAX_RESOURCES], int allot[][MAX_RESOURCES], int request[], int p) {
    int need[MAX_PROCESSES][MAX_RESOURCES];
    calculateNeed(need, max, allot);

    for (int j = 0; j < MAX_RESOURCES; j++) {
        if (request[j] > need[p][j]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return false;
        }
        if (request[j] > avail[j]) {
            printf("Process is not allocated resources. Request cannot be granted.\n");
            return false;
        }
    }

    
    for (int j = 0; j < MAX_RESOURCES; j++) {
        avail[j] -= request[j];
        allot[p][j] += request[j];
    }

    
    if (isSafe(processes, numProcesses, avail, max, allot)) {
        printf("Request granted.\n");
        return true;
    } else {
    
        for (int j = 0; j < MAX_RESOURCES; j++) {
            avail[j] += request[j];
            allot[p][j] -= request[j];
        }
        printf("Request cannot be granted as it leads to unsafe state.\n");
        return false;
    }
}

int main() {
    int processes[] = {0, 1, 2, 3, 4};
    int numProcesses = sizeof(processes) / sizeof(processes[0]);

    int max[MAX_PROCESSES][MAX_RESOURCES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    int allot[MAX_PROCESSES][MAX_RESOURCES] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int avail[MAX_RESOURCES] = {3, 3, 2};

    
    printMatrices(max, allot, avail);

    
    isSafe(processes, numProcesses, avail, max, allot);

    
    int request1[MAX_RESOURCES] = {1, 0, 2}; // Request from P1
    requestResources(processes, numProcesses, avail, max, allot, request1, 0);
    printMatrices(max, allot, avail);  // Print available resources after P1's request

    
    int request2[MAX_RESOURCES] = {0, 2, 0}; // Request from P2
    requestResources(processes, numProcesses, avail, max, allot, request2, 1);
    printMatrices(max, allot, avail);  // Print available resources after P2's request

    return 0;
}
