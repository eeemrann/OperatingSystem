#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Task {
    string taskName;
    int arrivalTime, burstTime, priority, completionTime, turnaroundTime, waitingTime;
};

// Priority Scheduling (Non-Preemptive)
void priorityScheduling(vector<Task> &tasks) {
    sort(tasks.begin(), tasks.end(), [](Task &a, Task &b) {
        if (a.arrivalTime == b.arrivalTime) return a.priority < b.priority;
        return a.arrivalTime < b.arrivalTime;
    });
    
    int currentTime = 0;
    vector<pair<int, string>> ganttChart;
    
    for (auto &task : tasks) {
        if (currentTime < task.arrivalTime) {
            ganttChart.push_back({currentTime, "Idle"});
            currentTime = task.arrivalTime;
        }
        ganttChart.push_back({currentTime, task.taskName});
        task.completionTime = currentTime + task.burstTime;
        task.turnaroundTime = task.completionTime - task.arrivalTime;
        task.waitingTime = task.turnaroundTime - task.burstTime;
        currentTime = task.completionTime;
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
    cout << "\nTask\tAT\tBT\tPrio\tCT\tTT\tWT\n";
    for (auto &task : tasks) {
        cout << task.taskName << "\t" << task.arrivalTime << "\t" << task.burstTime << "\t"
             << task.priority << "\t" << task.completionTime << "\t" << task.turnaroundTime << "\t"
             << task.waitingTime << "\n";
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
        cout << "Enter Priority for " << tasks[i].taskName << " (Lower number = Higher priority): ";
        cin >> tasks[i].priority;
    }
    
    priorityScheduling(tasks);
    printGanttChart(tasks);
    printResults(tasks);
    return 0;
}
