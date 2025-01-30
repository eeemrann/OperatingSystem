#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Task {
    string taskName;
    int arrivalTime, burstTime, completionTime, turnaroundTime, waitingTime;
};

// SJF Scheduling (Non-Preemptive)
void sjf(vector<Task> &tasks) {
    int n = tasks.size();
    vector<bool> completed(n, false);
    int currentTime = 0, completedTasks = 0;

    vector<pair<int, string>> ganttChart;
    
    while (completedTasks < n) {
        int minIdx = -1, minBT = 1e9;
        
        // Find the shortest job that has arrived
        for (int i = 0; i < n; i++) {
            if (!completed[i] && tasks[i].arrivalTime <= currentTime) {
                if (tasks[i].burstTime < minBT) {
                    minBT = tasks[i].burstTime;
                    minIdx = i;
                }
            }
        }
        
        if (minIdx == -1) {
            ganttChart.push_back({currentTime, "Idle"});
            currentTime++;
        } else {
            ganttChart.push_back({currentTime, tasks[minIdx].taskName});
            currentTime += tasks[minIdx].burstTime;
            tasks[minIdx].completionTime = currentTime;
            tasks[minIdx].turnaroundTime = tasks[minIdx].completionTime - tasks[minIdx].arrivalTime;
            tasks[minIdx].waitingTime = tasks[minIdx].turnaroundTime - tasks[minIdx].burstTime;
            
            completed[minIdx] = true;
            completedTasks++;
        }
    }
    
    // Print Gantt Chart
    cout << "\nGantt Chart:\n|";
    for (auto &entry : ganttChart) {
        cout << " " << entry.second << " |";
    }
    cout << "\n0";
    for (auto &entry : ganttChart) {
        cout << "\t" << entry.first;
    }
    cout << "\n";
}

// Print results
void printResults(vector<Task> &tasks) {
    double totalTurnaroundTime = 0, totalWaitingTime = 0;
    cout << "\nTask\tAT\tBT\tCT\tTT\tWT\n";
    for (auto &task : tasks) {
        cout << task.taskName << "\t" << task.arrivalTime << "\t" << task.burstTime << "\t"
             << task.completionTime << "\t" << task.turnaroundTime << "\t" << task.waitingTime << "\n";
        totalTurnaroundTime += task.turnaroundTime;
        totalWaitingTime += task.waitingTime;
    }
    
    cout << fixed << setprecision(2);
    cout << "\nAverage Turnaround Time: " << totalTurnaroundTime / tasks.size();
    cout << "\nAverage Waiting Time: " << totalWaitingTime / tasks.size() << "\n";
}

int main() {
    int numTasks;
    cout << "Enter number of tasks: ";
    cin >> numTasks;
    vector<Task> tasks(numTasks);
    
    for (int i = 0; i < numTasks; i++) {
        tasks[i].taskName = "P" + to_string(i + 1);
        cout << "Enter Arrival Time for " << tasks[i].taskName << ": ";
        cin >> tasks[i].arrivalTime;
        cout << "Enter Burst Time for " << tasks[i].taskName << ": ";
        cin >> tasks[i].burstTime;
    }
    
    sjf(tasks);
    printResults(tasks);
    return 0;
}
