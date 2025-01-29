#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;

struct Process {
    string name;
    int arrival, burst, remaining, completion, turnaround, waiting;
};

struct Task
{
    string taskName;
    int arrivalTime, burstTime, completionTime, turnaroundTime, waitingTime, remainingBurstTime;
};

int selectProcess(vector<Task> &tasks, int currentTime)
{
    int selectedIdx = -1;
    for (int i = 0; i < tasks.size(); i++)
    {
        if (tasks[i].remainingBurstTime > 0 && tasks[i].arrivalTime <= currentTime)
        {
            if (selectedIdx == -1 ||
                tasks[i].remainingBurstTime < tasks[selectedIdx].remainingBurstTime ||
                (tasks[i].remainingBurstTime == tasks[selectedIdx].remainingBurstTime && tasks[i].arrivalTime < tasks[selectedIdx].arrivalTime))
            {
                selectedIdx = i;
            }
        }
    }
    return selectedIdx;
}

void fcfs(vector<Process> &processes, vector<pair<int, string>> &gantt, double &totalTT, double &totalWT) {
    int time = 0;
    for (auto &p : processes) {
        if (time < p.arrival) {
            gantt.push_back({time, "Idle"});
            time = p.arrival;
        }
        gantt.push_back({time, p.name});
        p.completion = time + p.burst;
        p.turnaround = p.completion - p.arrival;
        p.waiting = p.turnaround - p.burst;
        totalTT += p.turnaround;
        totalWT += p.waiting;
        time = p.completion;
    }
}

void sjf(vector<Process> &processes, vector<pair<int, string>> &gantt, double &totalTT, double &totalWT) {
    sort(processes.begin(), processes.end(), [](Process &a, Process &b) { 
        return (a.arrival < b.arrival) || (a.arrival == b.arrival && a.burst < b.burst); 
    });
    
    int time = 0, completedCount = 0, numProcesses = processes.size();
    for (auto &p : processes) p.remaining = p.burst;
    
    while (completedCount < numProcesses) {
        int idx = -1;
        for (int i = 0; i < numProcesses; i++) {
            if (processes[i].arrival <= time && processes[i].remaining > 0) {
                if (idx == -1 || processes[i].remaining < processes[idx].remaining)
                    idx = i;
            }
        }

        if (idx == -1) {
            gantt.push_back({time, "Idle"});
            time++;
        } else {
            gantt.push_back({time, processes[idx].name});
            processes[idx].remaining--;
            time++;

            if (processes[idx].remaining == 0) {
                processes[idx].completion = time;
                processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
                totalTT += processes[idx].turnaround;
                totalWT += processes[idx].waiting;
                completedCount++;
            }
        }
    }
}

void srtf(vector<Process> &processes, vector<pair<int, string>> &gantt, double &totalTT, double &totalWT) {
    int time = 0, done = 0, n = processes.size();
    for (auto &p : processes) p.remaining = p.burst;
    while (done < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= time && processes[i].remaining > 0) {
                if (idx == -1 || processes[i].remaining < processes[idx].remaining)
                    idx = i;
            }
        }
        if (idx == -1) {
            gantt.push_back({time, "Idle"});
            time++;
        } else {
            gantt.push_back({time, processes[idx].name});
            processes[idx].remaining--;
            time++;
            if (processes[idx].remaining == 0) {
                processes[idx].completion = time;
                processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
                totalTT += processes[idx].turnaround;
                totalWT += processes[idx].waiting;
                done++;
            }
        }
    }
}

void printGanttChart(const vector<pair<int, string>> &gantt, const string &algorithm) {
    cout << "\n" << algorithm << "\n";
    cout << "\nGantt Chart:\n";
    cout << "Time:  ";
    for (const auto &entry : gantt) {
        cout << setw(4) << entry.first << " ";
    }
    cout << "\n       ";
    for (size_t i = 0; i < gantt.size(); i++) {
        cout << "+----";
    }
    cout << "+\n       ";
    for (const auto &entry : gantt) {
        cout << "| " << setw(2) << entry.second << " ";
    }
    cout << "|\n       ";
    for (size_t i = 0; i < gantt.size(); i++) {
        cout << "+----";
    }
    cout << "+\n";
}

void printResults(const vector<Process> &processes, double totalTT, double totalWT) {
    cout << "\nProcess\tAT\tBT\tCT\tTT\tWT\n";
    for (const auto &p : processes) {
        cout << p.name << "\t" << p.arrival << "\t" << p.burst << "\t" << p.completion << "\t" << p.turnaround << "\t" << p.waiting << "\n";
    }
    cout << fixed << setprecision(2);
    cout << "\nAverage Turnaround Time: " << totalTT / processes.size();
    cout << "\nAverage Waiting Time: " << totalWT / processes.size() << "\n";
}

int main() {
    int numProcesses, choice;
    cout << "Scheduling Algorithm:\n1. FCFS\n2. SJF\n3. SRTF\nEnter an option: ";
    cin >> choice;
    cout << "Enter number of processes: ";
    cin >> numProcesses;
    vector<Process> processes(numProcesses);
    for (int i = 0; i < numProcesses; i++) {
        processes[i].name = "P" + to_string(i + 1);
        cout << "Enter Arrival Time for " << processes[i].name << ": ";
        cin >> processes[i].arrival;
        cout << "Enter Burst Time for " << processes[i].name << ": ";
        cin >> processes[i].burst;
    }
    vector<pair<int, string>> gantt;
    double totalTT = 0, totalWT = 0;
    string algorithm;
    if (choice == 1) {
        algorithm = "FCFS";
        fcfs(processes, gantt, totalTT, totalWT);
    } else if (choice == 2) {
        algorithm = "SJF";
        sjf(processes, gantt, totalTT, totalWT);
    } else if (choice == 3) {
        algorithm = "SRTF";
        srtf(processes, gantt, totalTT, totalWT);
    }
    printGanttChart(gantt, algorithm);
    printResults(processes, totalTT, totalWT);
    return 0;
}
