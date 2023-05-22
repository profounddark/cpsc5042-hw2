#include <iostream>
#include <pthread.h>
#include <mutex>




using std::cout, std::cin, std::endl;

const int MAX_THREADS = 3;

int allowed_ID = 1;
std::mutex threadLock;

void * threadFunction(void * thread_ID) {
    int * this_ID = (int *) thread_ID;
    int counter = 0;

    while (counter < 2) {
        threadLock.lock();
        if (allowed_ID == *this_ID) {
            cout << "Thread " << *this_ID << "'s turn!" << endl;
            counter++;
            allowed_ID++;
            if (allowed_ID == (MAX_THREADS + 1)) {
                allowed_ID = 1;
            }
        } else {
            cout << "Not thread " << *this_ID << "'s turn!" << endl;
        }
        threadLock.unlock();
    }
    cout << "Thread " << *this_ID << " complete." << endl;

}

int main() {
    int idCount[MAX_THREADS];
    pthread_t threadArray[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++) {
        idCount[i] = i + 1;
        pthread_create(&threadArray[i], nullptr, threadFunction, &idCount[i]);
    }
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threadArray[i], nullptr);
    }





    return 0;
}