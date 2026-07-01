#include <stdio.h>
#include <time.h>

// Cross-platform non-blocking sleep definitions
#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} ProcessState;

typedef struct {
    int id;
    int timeLeft;
    ProcessState state;
} Process;

// Space: O(1) | Time: O(1) non-blocking resource suspend
void executeDelay(int milliseconds) {
    sleep_ms(milliseconds);
}

// Space: O(1) Auxiliary | Time: O(N * (Total Burst Time / Quantum))
void runRoundRobin(Process processes[], int numProcesses) {
    int quantum = 10;
    int incompleteProcesses = 0;
    int currentTime = 0;
    int contextSwitchTime = 2;

    for (int i = 0; i < numProcesses; i++) {
        if (processes[i].state == READY) {
            currentTime += contextSwitchTime; // Simulate overhead cost
            printf("Time %d: Process %d changes state to RUNNING\n", currentTime, processes[i].id);
            processes[i].state = RUNNING;

            if (processes[i].timeLeft > quantum) {
                incompleteProcesses++;
                processes[i].timeLeft -= quantum;
                processes[i].state = READY;
                currentTime += quantum;
                
                executeDelay(quantum); // Suspend execution thread safely
                printf("Process %d quantum expired. Remaining time: %d ms at Global Time: %d\n\n", 
                       processes[i].id, processes[i].timeLeft, currentTime);
            } 
            else {
                processes[i].state = TERMINATED;
                currentTime += processes[i].timeLeft;
                
                executeDelay(processes[i].timeLeft); // Suspend execution thread safely
                printf("Process %d completed execution at Global Time: %d\n\n", processes[i].id, currentTime);
            }
        }

        // Circular queue emulation logic
        if (i == numProcesses - 1) {
            if (incompleteProcesses >= 1) {
                i = -1; // Reset to index 0 on next loop increment step
                incompleteProcesses = 0;
            }
        }
    }
}

int main() {
    Process processes[] = {
        {1, 14, READY},
        {2, 2,  READY},
        {3, 60, READY},
        {4, 8,  READY}
    }; 

    int numProcesses = sizeof(processes) / sizeof(processes[0]); // Space: O(1) | Time: O(1)
    
    printf("Starting Round Robin CPU Scheduling Simulation...\n");
    printf("Quantum Time Slice: 10ms | Context Switch Overhead: 2ms\n\n");
    
    runRoundRobin(processes, numProcesses);
    
    printf("Simulation Complete. All execution sequences terminated successfully.\n");
    return 0;
}