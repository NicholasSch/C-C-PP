# 🕒 Round Robin CPU Scheduling Simulator

A lightweight, zero-dependency C console application that simulates a low-level operating system CPU task scheduler using a Round Robin algorithm with context-switch time penalties.

---

## 📄 Project Overview

This program simulates a time-sliced multiprocessing kernel environment. Given an array of processing tasks (Processes) initialized with individual execution requirements (Burst Times), the scheduling engine cycles through active processes, giving each a fixed duration window (Quantum).

The application uses an automated cross-platform preprocessor architecture that maps non-blocking sleep hooks (`Sleep` on Windows, `usleep` on POSIX environments). This configuration avoids heavy busy-wait polling loops, ensuring the system runs efficiently without wasting CPU cycles during execution delays.

### Core Objectives
* **Circular Queue Emulation:** Iterating across active process arrays dynamically to mimic ring-buffer hardware queues.
* **Context Switch Modeling:** Simulating operational scheduling overhead parameters by injecting fixed clock penalties before assigning execution states.
* **Stateless Multiplatform Portability:** Writing code without relying on bulky third-party libraries to guarantee simple compilations across various platforms.

---

## 🛡️ Algorithmic Complexity Profile

### Resource Utilization Bounds
* **Total Time Complexity:** $O(N \cdot (T / Q))$ worst-case, where `N` is the number of processes, `T` is the maximum burst execution duration present inside the workload, and `Q` is the quantum time slice. The scheduler cycles through incomplete items iteratively until all states reach their termination condition.
* **Total Space Complexity:** $O(1)$ auxiliary memory space. The execution loop mutates tracking elements directly in place within the input reference arrays, requiring zero extra runtime data-structure expansions.

---

## 📝 Formal Pseudocode (Round Robin Kernel Scheduler)

This structured English pseudocode covers the scheduling evaluation pass logic and index recycling constraints:

```text
STRUCT Process:
    property id
    property timeLeft
    property state

FUNCTION RunRoundRobin(processArray, numProcesses):
    quantum = 10
    incompleteCount = 0
    currentTime = 0
    contextSwitchCost = 2

    FOR index FROM 0 TO numProcesses - 1:
        IF processArray[index].state EQUALS READY THEN:
            currentTime = currentTime + contextSwitchCost
            PrintStatus(currentTime, processArray[index].id, "RUNNING")
            processArray[index].state = RUNNING

            // Check if process demands more time than the quantum slice allows
            IF processArray[index].timeLeft > quantum THEN:
                incompleteCount = incompleteCount + 1
                processArray[index].timeLeft = processArray[index].timeLeft - quantum
                processArray[index].state = READY
                currentTime = currentTime + quantum
                
                SuspendThreadExecution(quantum)
                PrintStatus("Quantum Expired", processArray[index].id, currentTime)
            
            // Process can finish within the current quantum time slice
            ELSE:
                processArray[index].state = TERMINATED
                currentTime = currentTime + processArray[index].timeLeft
                
                SuspendThreadExecution(processArray[index].timeLeft)
                PrintStatus("Terminated", processArray[index].id, currentTime)

        // Queue boundary check: Evaluate index recycling needs
        IF index EQUALS numProcesses - 1 THEN:
            IF incompleteCount >= 1 THEN:
                index = -1 // Reset loop pointer back to start on next increment
                incompleteCount = 0
```

---

## 🛠️ Prerequisites & Environment Setup

Before compiling the code, you must ensure a native C compiler toolchain (`gcc` or `clang`) is installed on your operating system.

### 🪟 Windows (via MSYS2)
1. Download and run the **MSYS2** installer from [msys2.org](https://www.msys2.org/).
2. Open the MSYS2 UCRT64 terminal environment and execute the following command to download the GCC compiler toolchain:
   ```bash
   pacman -S mingw-w64-x86_64-toolchain
   ```
3. Add the compiler binaries directory to your system's Environment Variables **Path**:
   * Open the Windows Start Menu, search for **"Environment Variables"**, and press Enter.
   * Edit the `Path` variable under *System Variables* and append: `C:\msys64\mingw64\bin` (or `C:\msys64\ucrt64\bin` depending on your environment).
4. Restart your IDE or terminal workspace to reload the updated system paths.

### 🍎 macOS
1. Open your native terminal app and trigger the command line developer tools downloader package by executing:
   ```bash
   xcode-select --install
   ```
2. Verify the installation completed successfully by running: `gcc --version` or `clang --version`.

### 🐧 Linux (Ubuntu / Debian)
1. Synchronize package indexes and acquire the essential compiler utilities using your native package manager:
   ```bash
   sudo apt update
   sudo apt install build-essential
   ```

---

## 🚀 Execution Instructions

Once your toolchain environment is configured, navigate to your project directory containing your source code asset (`main.c`) and execute these commands in your machine terminal panel to compile and run:

```bash
# 1. Compile the source file into a native executable binary
gcc main.c -o scheduler_sim

# 2. Execute the compiled simulator binary
# On Windows:
.\scheduler_sim

# On macOS / Linux:
./scheduler_sim
```
