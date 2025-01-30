#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
using namespace std;

struct Process {
    string name;
    int arrival, burst, priority, quantum, remaining, completion, turnaround, waiting;
};

void printGanttChart(const vector<pair<int, string>> &gantt) {
    cout << "\nGantt Chart:\n";
    for (const auto &entry : gantt) {
        cout << "| " << entry.second << " ";
    }
    cout << "|\n";
    for (const auto &entry : gantt) {
        cout << entry.first << "    ";
    }
    cout << "\n";
}

void printResults(vector<Process> &processes, double totalTT, double totalWT, int numProcesses) {
    cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    for (const auto &p : processes) {
        cout << p.name << "\t" << p.arrival << "\t" << p.burst << "\t" << p.completion << "\t" << p.turnaround << "\t" << p.waiting << "\n";
    }
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << totalTT / numProcesses << "\n";
    cout << "Average Waiting Time: " << fixed << setprecision(2) << totalWT / numProcesses << "\n";
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
    sort(processes.begin(), processes.end(), [](Process &a, Process &b) { 
        return a.arrival < b.arrival; 
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

void priorityScheduling(vector<Process> &processes, vector<pair<int, string>> &gantt, double &totalTT, double &totalWT) {
    sort(processes.begin(), processes.end(), [](Process &a, Process &b) { 
        return a.arrival < b.arrival; 
    });
    
    int time = 0, completedCount = 0, numProcesses = processes.size();
    for (auto &p : processes) p.remaining = p.burst;
    
    while (completedCount < numProcesses) {
        int idx = -1;
        for (int i = 0; i < numProcesses; i++) {
            if (processes[i].arrival <= time && processes[i].remaining > 0) {
                if (idx == -1 || processes[i].priority < processes[idx].priority)
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

void roundRobin(vector<Process> &processes, vector<pair<int, string>> &gantt, double &totalTT, double &totalWT, int quantum) {
    sort(processes.begin(), processes.end(), [](Process &a, Process &b) { 
        return a.arrival < b.arrival; 
    });
    
    int time = 0, completedCount = 0, numProcesses = processes.size();
    queue<int> q;
    for (auto &p : processes) p.remaining = p.burst;
    
    while (completedCount < numProcesses) {
        for (int i = 0; i < numProcesses; i++) {
            if (processes[i].arrival <= time && processes[i].remaining > 0) {
                q.push(i);
            }
        }

        if (q.empty()) {
            gantt.push_back({time, "Idle"});
            time++;
        } else {
            int idx = q.front();
            q.pop();
            gantt.push_back({time, processes[idx].name});
            int execTime = min(quantum, processes[idx].remaining);
            processes[idx].remaining -= execTime;
            time += execTime;
            if (processes[idx].remaining == 0) {
                processes[idx].completion = time;
                processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
                totalTT += processes[idx].turnaround;
                totalWT += processes[idx].waiting;
                completedCount++;
            } else {
                q.push(idx);
            }
        }
    }
}

int main() {
    int numProcesses, choice, quantum;
    cout << "Scheduling Algorithm:\n1. FCFS\n2. SJF\n3. SRTF\n4. Priority Scheduling\n5. Round Robin\nEnter an option: ";
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
        if (choice == 4) {
            cout << "Enter Priority for " << processes[i].name << ": ";
            cin >> processes[i].priority;
        }
        processes[i].remaining = processes[i].burst;
    }
    if (choice == 5) {
        cout << "Enter Quantum: ";
        cin >> quantum;
    }
    vector<pair<int, string>> gantt;
    double totalTT = 0, totalWT = 0;
    if (choice == 1) fcfs(processes, gantt, totalTT, totalWT);
    else if (choice == 2) sjf(processes, gantt, totalTT, totalWT);
    else if (choice == 3) srtf(processes, gantt, totalTT, totalWT);
    else if (choice == 4) priorityScheduling(processes, gantt, totalTT, totalWT);
    else if (choice == 5) roundRobin(processes, gantt, totalTT, totalWT, quantum);
    printGanttChart(gantt);
    printResults(processes, totalTT, totalWT, numProcesses);
    return 0;
}