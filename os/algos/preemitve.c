#include <stdio.h>
#include <limits.h>

#define MAX_PROCESSES 4
#define QUANTUM 30

typedef struct {
    int id;
    int priority;
    int burst_time;
    int arrival_time;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
} Process;

void calculate_turnaround_time(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
    }
}

void calculate_waiting_time(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
    }
}

void shortest_remaining_time(Process processes[], int n) {
    int time = 0;
    int completed = 0;
    while (completed < n) {
        int min_time = INT_MAX;
        int idx = -1;
        
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0 && processes[i].remaining_time < min_time) {
                min_time = processes[i].remaining_time;
                idx = i;
            }
        }

        if (idx != -1) {
            time++;
            processes[idx].remaining_time--;
            if (processes[idx].remaining_time == 0) {
                completed++;
                processes[idx].completion_time = time;
            }
        } else {
            time++;
        }
    }

    calculate_turnaround_time(processes, n);
    calculate_waiting_time(processes, n);
}

void non_preemptive_priority(Process processes[], int n) {
    int time = 0, completed = 0;

    while (completed < n) {
        int highest_priority = INT_MAX;
        int idx = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0 && processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                idx = i;
            }
        }

        if (idx != -1) {
            time += processes[idx].remaining_time;
            processes[idx].remaining_time = 0;
            processes[idx].completion_time = time;
            completed++;
        } else {
            time++;
        }
    }

    calculate_turnaround_time(processes, n);
    calculate_waiting_time(processes, n);
}

void round_robin(Process processes[], int n) {
    int time = 0, completed = 0;
    int all_completed = 0;

    while (completed < n) {
        all_completed = 1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0) {
                all_completed = 0;
                if (processes[i].remaining_time > QUANTUM) {
                    time += QUANTUM;
                    processes[i].remaining_time -= QUANTUM;
                } else {
                    time += processes[i].remaining_time;
                    processes[i].remaining_time = 0;
                    processes[i].completion_time = time;
                    completed++;
                }
            }
        }

        if (all_completed) time++;
    }

    calculate_turnaround_time(processes, n);
    calculate_waiting_time(processes, n);
}

void print_results(Process processes[], int n) {
    float total_waiting_time = 0;
    printf("Process\tPriority\tBurst\tArrival\tCompletion\tTurnaround\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t%d\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].priority,
               processes[i].burst_time, processes[i].arrival_time, processes[i].completion_time,
               processes[i].turnaround_time, processes[i].waiting_time);
        total_waiting_time += processes[i].waiting_time;
    }

    printf("Average Waiting Time: %.2f ms\n", total_waiting_time / n);
}

int main() {
    Process processes[MAX_PROCESSES] = {
        {0, 4, 50, 0, 50, 0, 0, 0},
        {1, 1, 20, 20, 20, 0, 0, 0},
        {2, 3, 100, 40, 100, 0, 0, 0},
        {3, 2, 40, 60, 40, 0, 0, 0}
    };

    printf("Shortest Remaining Time Scheduling:\n");
    shortest_remaining_time(processes, MAX_PROCESSES);
    print_results(processes, MAX_PROCESSES);

    // Reset remaining times for next scheduling
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completion_time = 0;
    }

    printf("\nNon-Preemptive Priority Scheduling:\n");
    non_preemptive_priority(processes, MAX_PROCESSES);
    print_results(processes, MAX_PROCESSES);

    // Reset remaining times for next scheduling
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completion_time = 0;
    }

    printf("\nRound Robin Scheduling (Quantum = 30 ms):\n");
    round_robin(processes, MAX_PROCESSES);
    print_results(processes, MAX_PROCESSES);

    return 0;
}

