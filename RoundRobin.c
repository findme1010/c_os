#include <stdio.h>
#include <stdlib.h>

typedef struct process {
    int Id, AT, BT, CT, TAT, WT, flag;
} pro;

pro p[15];

void swap(pro *a, pro *b) {
    pro temp = *a;
    *a = *b;
    *b = temp;
}

void sort(int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].AT > p[j + 1].AT) {
                swap(&p[j], &p[j + 1]);
            }
        }
    }
}

int main() {
    int n, tempBT[15], total_WT = 0, total_TAT = 0, quantum;
    float avg_WT = 0, avg_TAT = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("(AT BT):\n");
    for (int i = 0; i < n; i++) {
        p[i].Id = i + 1;
        scanf("%d%d", &p[i].AT, &p[i].BT);
        tempBT[i] = p[i].BT; // Keep original BT for WT calculation
        p[i].flag = 0;       // Not yet in queue
    }

    printf("Enter the Time Quantum: ");
    scanf("%d", &quantum);

    sort(n);

    int completed = 0, curTime = p[0].AT;
    int queue[100], front = 0, rear = 0;
    int visited[15] = {0};

    queue[rear++] = 0;
    visited[0] = 1;

    printf("\nGantt Chart:\n");
    while (completed != n) {
        int idx = queue[front++];
        if (p[idx].BT > quantum) {
            printf("| P%d(%d) %d ", p[idx].Id, quantum, curTime + quantum);
            p[idx].BT -= quantum;
            curTime += quantum;
        } else if (p[idx].BT > 0) {
            printf("| P%d(%d) %d ", p[idx].Id, p[idx].BT, curTime + p[idx].BT);
            curTime += p[idx].BT;
            p[idx].BT = 0;
            p[idx].CT = curTime;
            p[idx].TAT = p[idx].CT - p[idx].AT;
            p[idx].WT = p[idx].TAT - tempBT[p[idx].Id - 1];
            total_TAT += p[idx].TAT;
            total_WT += p[idx].WT;
            completed++;
        }

        // Check new processes that arrived during this time
        for (int i = 0; i < n; i++) {
            if (p[i].AT <= curTime && !visited[i] && p[i].BT > 0) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        // If the current process still has burst time, put it back in queue
        if (p[idx].BT > 0) {
            queue[rear++] = idx;
        }

        // If queue becomes empty, move time forward to next arriving process
        if (front == rear && completed < n) {
            for (int i = 0; i < n; i++) {
                if (p[i].BT > 0) {
                    queue[rear++] = i;
                    visited[i] = 1;
                    curTime = p[i].AT > curTime ? p[i].AT : curTime;
                    break;
                }
            }
        }
    }

    avg_TAT = (float)total_TAT / n;
    avg_WT = (float)total_WT / n;

    printf("\n\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].Id, p[i].AT, tempBT[i], p[i].CT, p[i].TAT, p[i].WT);
    }

    printf("\nAverage Turnaround Time = %.2f\n", avg_TAT);
    printf("Average Waiting Time = %.2f\n", avg_WT);

    return 0;
}
