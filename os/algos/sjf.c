#include <stdio.h>

void findWaitingTime(int n, int bt[], int wt[]) {
    wt[0] = 0;  // Waiting time for the first process is 0
    for (int i = 1; i < n; i++) {
        wt[i] = bt[i - 1] + wt[i - 1];
    }
}

void findTurnAroundTime(int n, int bt[], int wt[], int tat[]) {
    for (int i = 0; i < n; i++) {
        tat[i] = bt[i] + wt[i];
    }
}

void findAvgTime(int processes[], int n, int bt[]) {
    int wt[n], tat[n], total_wt = 0, total_tat = 0;

    findWaitingTime(n, bt, wt);
    findTurnAroundTime(n, bt, wt, tat);

    printf("\n");
    printf("Processes   Burst Time   Waiting Time   Turnaround Time\n");
    printf("--------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        printf("    %d          %2d            %2d              %2d\n", 
               processes[i], bt[i], wt[i], tat[i]);
    }
    
    printf("--------------------------------------------------------\n");
    printf("Average Waiting Time: %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time: %.2f\n\n", (float)total_tat / n);
}

void sortProcesses(int processes[], int n, int bt[]) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (bt[j] > bt[j + 1]) {
                // Swap burst time
                int temp_bt = bt[j];
                bt[j] = bt[j + 1];
                bt[j + 1] = temp_bt;

                // Swap process number
                int temp_process = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp_process;
            }
        }
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    int processes[n], burst_time[n];
    for (int i = 0; i < n; i++) {
        processes[i] = i + 1; // Assign process ID starting from 1
        printf("Enter burst time for process %d: ", processes[i]);
        scanf("%d", &burst_time[i]);
    }
    sortProcesses(processes, n, burst_time);
    findAvgTime(processes, n, burst_time);
    return 0;
}
