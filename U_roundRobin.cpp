#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
using namespace std;

struct Task {
    string taskName;
    int arrivalTime, burstTime, remainingTime, completionTime, turnaroundTime, waitingTime;
};

// Round Robin Scheduling
void roundRobinScheduling(vector<Task> &tasks, int timeQuantum) {
    int currentTime = 0;
    queue<int> readyQueue;
    vector<pair<int, string>> ganttChart;
    int remainingTasks = tasks.size();
    
    for (auto &task : tasks) {
        task.remainingTime = task.burstTime;
    }
    
    auto compareArrival = [](Task &a, Task &b) { return a.arrivalTime < b.arrivalTime; };
    sort(tasks.begin(), tasks.end(), compareArrival);
    
    int index = 0;
    while (remainingTasks > 0) {
        while (index < tasks.size() && tasks[index].arrivalTime <= currentTime) {
            readyQueue.push(index);
            index++;
        }
        
        if (readyQueue.empty()) {
            ganttChart.push_back({currentTime, "Idle"});
            currentTime++;
        } else {
            int taskIdx = readyQueue.front();
            readyQueue.pop();
            
            ganttChart.push_back({currentTime, tasks[taskIdx].taskName});
            int executeTime = min(timeQuantum, tasks[taskIdx].remainingTime);
            tasks[taskIdx].remainingTime -= executeTime;
            currentTime += executeTime;
            
            while (index < tasks.size() && tasks[index].arrivalTime <= currentTime) {
                readyQueue.push(index);
                index++;
            }
            
            if (tasks[taskIdx].remainingTime > 0) {
                readyQueue.push(taskIdx);
            } else {
                tasks[taskIdx].completionTime = currentTime;
                tasks[taskIdx].turnaroundTime = tasks[taskIdx].completionTime - tasks[taskIdx].arrivalTime;
                tasks[taskIdx].waitingTime = tasks[taskIdx].turnaroundTime - tasks[taskIdx].burstTime;
                remainingTasks--;
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
             << task.completionTime << "\t" << task.turnaroundTime << "\t"
             << task.waitingTime << "\n";
        totalTurnaroundTime += task.turnaroundTime;
        totalWaitingTime += task.waitingTime;
    }
    
    cout << fixed << setprecision(2);
    cout << "\nAverage Turnaround Time: " << totalTurnaroundTime / tasks.size();
    cout << "\nAverage Waiting Time: " << totalWaitingTime / tasks.size() << "\n";
}

int main() {
    int numTasks, timeQuantum;
    cout << "Enter number of tasks: ";
    cin >> numTasks;
    cout << "Enter Time Quantum: ";
    cin >> timeQuantum;
    vector<Task> tasks(numTasks);
    
    for (int i = 0; i < numTasks; i++) {
        tasks[i].taskName = "P" + to_string(i + 1);
        cout << "Enter Arrival Time for " << tasks[i].taskName << ": ";
        cin >> tasks[i].arrivalTime;
        cout << "Enter Burst Time for " << tasks[i].taskName << ": ";
        cin >> tasks[i].burstTime;
    }
    
    roundRobinScheduling(tasks, timeQuantum);
    printGanttChart(tasks);
    printResults(tasks);
    return 0;
}
