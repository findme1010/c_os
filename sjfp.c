#include <stdio.h>

typedef struct process {
    int pid;
    int AT;
    int BT;
    int RemT;
    int CT;
    int TAT;
    int WT;
    int RT;
    int isStarted;
} Process;

void sjfPreemptive(Process[], int);

int main() {
    int n;
    printf("Enter the number of p: ");
    scanf("%d", &n);
    
    Process p[n];

    for (int i = 0; i < n; i++) {
        printf("Process %d\n", i + 1);
        printf("Enter Arrival Time: ");
        scanf("%d", &p[i].AT);
        printf("Enter Burst Time: ");
        scanf("%d", &p[i].BT);
        
        p[i].pid = i + 1;
        p[i].RemT = p[i].BT;
        p[i].isStarted = 0;
        printf("\n");
    }

    sjfPreemptive(p, n);
    return 0;
}

void sjfPreemptive(Process p[], int n) {
    int timeProgress[100], processTrack[100], j = -1, k = -1;
    float avgWT = 0, avgTAT = 0, avgRT = 0;
    int totalWT = 0, totalTAT = 0, totalRT = 0;
    int elapsedTime = 0, remainingp = n;
    
    timeProgress[++j] = 0;

    while (remainingp) {
        int exec = -1;
        int shortestBT = 9999;

        for (int i = 0; i < n; i++) {
            if (p[i].AT <= elapsedTime && p[i].RemT > 0) {
                if (p[i].RemT < shortestBT) {
                    shortestBT = p[i].RemT;
                    exec = i;
                }
            }
        }

        if (exec == -1) {
            processTrack[++k] = 0; // 0 means idle
            elapsedTime++;
            timeProgress[++j] = elapsedTime;
            continue;
        }

        if (p[exec].isStarted == 0) {
            p[exec].RT = elapsedTime - p[exec].AT;
            p[exec].isStarted = 1;
        }

        p[exec].RemT--;
        processTrack[++k] = p[exec].pid;
        elapsedTime++;
        timeProgress[++j] = elapsedTime;

        if (p[exec].RemT == 0) {
            p[exec].CT = elapsedTime;
            p[exec].TAT = p[exec].CT - p[exec].AT;
            p[exec].WT = p[exec].TAT - p[exec].BT;
            
            totalTAT += p[exec].TAT;
            totalWT += p[exec].WT;
            totalRT += p[exec].RT;

            remainingp--;
        }
    }

    avgTAT = (float)totalTAT / n;
    avgWT = (float)totalWT / n;
    avgRT = (float)totalRT / n;

    // Observation Table
    printf("\nObservation Table\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\tRemT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].AT,
               p[i].BT,
               p[i].CT,
               p[i].TAT,
               p[i].WT,
               p[i].RT);
    }

    // Gantt ChaRemT
    printf("\nGantt ChaRemT:\n(P0 --> idle time)\n");
    for (int i = 0; i <= k; i++) {
        printf("| P%d\t", processTrack[i]);
    }
    printf("|\n");
    for (int i = 0; i <= j; i++) {
        printf("%d\t", timeProgress[i]);
    }
    printf("\n");


    // Averages
    printf("\nAverage Waiting Time: %.2f\n", avgWT);
    printf("Average Turnaround Time: %.2f\n", avgTAT);
    printf("Average Response Time: %.2f\n", avgRT);
}
