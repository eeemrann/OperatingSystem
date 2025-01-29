#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Task {
    string taskName;
    int arrivalTime, burstTime, completionTime, turnaroundTime, waitingTime, remainingTime;
};

// SRTF Scheduling
void srtf(vector<Task> &tasks) {
    int currentTime = 0, completed = 0, n = tasks.size();
    int minIdx;
    for (auto &task : tasks) task.remainingTime = task.burstTime;
    
    vector<pair<int, string>> ganttChart;
    while (completed < n) {
        minIdx = -1;
        for (int i = 0; i < n; i++) {
            if (tasks[i].arrivalTime <= currentTime && tasks[i].remainingTime > 0) {
                if (minIdx == -1 || tasks[i].remainingTime < tasks[minIdx].remainingTime)
                    minIdx = i;
            }
        }
        
        if (minIdx == -1) {
            ganttChart.push_back({currentTime, "Idle"});
            currentTime++;
        } else {
            ganttChart.push_back({currentTime, tasks[minIdx].taskName});
            tasks[minIdx].remainingTime--;
            currentTime++;
            
            if (tasks[minIdx].remainingTime == 0) {
                tasks[minIdx].completionTime = currentTime;
                tasks[minIdx].turnaroundTime = tasks[minIdx].completionTime - tasks[minIdx].arrivalTime;
                tasks[minIdx].waitingTime = tasks[minIdx].turnaroundTime - tasks[minIdx].burstTime;
                completed++;
            }
        }
    }
}

// Print Gantt Chart
void printGanttChart(vector<Task> &tasks) {
    cout << "\nGantt Chart:\n|";
    for (auto &task : tasks) {
        cout << " " << task.taskName << " |";
    }
    cout << "\n0";
    for (auto &task : tasks) {
        cout << "\t" << task.completionTime;
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
    
    srtf(tasks);
    printGanttChart(tasks);
    printResults(tasks);
    return 0;
}
