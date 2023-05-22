#include <iostream>
#include <pthread.h>
#include <mutex>

// limit number of threads
const int MAX_THREADS = 3;


int allowed_ID = 1;
// using a mutex lock becuase otherwise the threads run over each other
std::mutex threadLock;

/**
 * threadFunction is the main method for each thread. It initializes a counter
 * and repeatedly checks the allowed_ID global to see if it is the thread's
 * "turn." If so, it announces that it is its turn and increments the counter.
 * Otherwise, it says it's not it's turn.
 * Implements a mutex-lock to prevent output overrun.
 * @param thread_ID the ID number for this specific thread
 * @return no return
 */
void * threadFunction(void * thread_ID) {

    // pointer to recast the thread ID
    int * this_ID = (int *) thread_ID;

    // counter for tracking how many times the thread has had a turn.
    int counter = 0;

    while (counter < 2) {
        // mutex lock to keep output sorted
        threadLock.lock();

        if (allowed_ID == *this_ID) {
            printf("Thread %d's turn!\n", *this_ID);
            counter++;
            allowed_ID++;
            if (allowed_ID == (MAX_THREADS + 1)) {
                allowed_ID = 1;
            }
        } else {
            printf("Not thread %d's turn!\n", *this_ID);
        }

        // unlock when complete
        threadLock.unlock();
    }
    printf("Thread %d complete.\n", *this_ID);

    return nullptr;
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