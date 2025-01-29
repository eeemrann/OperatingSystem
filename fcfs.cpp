#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Task {
    string taskName;
    int arrivalTime, burstTime, completionTime, turnaroundTime, waitingTime;
};

// FCFS Scheduling
void fcfs(vector<Task> &tasks) {
    sort(tasks.begin(), tasks.end(), [](Task &a, Task &b) { return a.arrivalTime < b.arrivalTime; });
    int currentTime = 0;
    for (auto &task : tasks) {
        if (currentTime < task.arrivalTime) currentTime = task.arrivalTime;
        task.completionTime = currentTime + task.burstTime;
        task.turnaroundTime = task.completionTime - task.arrivalTime;
        task.waitingTime = task.turnaroundTime - task.burstTime;
        currentTime = task.completionTime;
    }
}

// Print Gantt Chart
void printGanttChart(vector<Task> &tasks) {
    cout << "\nGantt Chart:\n";
    cout << "|";
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
    
    fcfs(tasks);
    printGanttChart(tasks);
    printResults(tasks);
    return 0;
}
