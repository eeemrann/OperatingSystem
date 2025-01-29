#include <iostream>
#include <thread>
#include <vector>
using namespace std;

int BUFFER_SIZE = 5;
vector<int> buff(BUFFER_SIZE);
int in = 0;
int out = 0;
int item = 0;

void producerFunc() {
    for (int i = 0; i < 5; i++) {
        buff[in] = ++item; // item is produced
        cout << "Produced item = " << item << " at index " << in << endl;
        in = (in + 1) % BUFFER_SIZE;
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void consumerFunc() {
    for (int i = 0; i < 5; i++) {
        cout << "Consumed item = " << item << " at index " << out << endl; // consumer consumes the item
        out = (out + 1) % BUFFER_SIZE;
        this_thread::sleep_for(chrono::milliseconds(80));
    }
}

