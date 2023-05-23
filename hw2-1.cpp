#include <iostream>
#include <pthread.h>
#include <vector>



using std::cout;
using std::endl;

// declare global variables for threads
int min, max;
float average;

/**
 * calcAverage calculates the average value of a vector of integers. Designed
 * for MULTI-THREADING.
 * @param param a integer vector full of numbers
 * @return nothing, but sets the global average variable to the average value
 * of the numbers in the vector
 */
void* calcAverage(void *param) {
    std::vector<int>* integerList = (std::vector<int>*) param;

    int sum = 0;
    for (int i = 0; i < integerList->size(); i++) {
        sum = sum + integerList->at(i);
    }
    average = sum / integerList->size();
    return nullptr;
}

/**
 * calcMin calculates the lowest number in a vector of integers. Designed
 * for MULTI-THREADING.
 * @param param a integer vector full of numbers
 * @return nothing, but sets the global min variable to the lowest value
 * in the vector
 */
void* calcMin(void *param) {
    std::vector<int>* integerList = (std::vector<int>*) param;

    min = integerList->at(0);
    for (int i = 1; i < integerList->size(); i++) {
        if (integerList->at(i) < min) {
            min = integerList->at(i);
        }
    }
    return nullptr;
}

/**
 * calcMax calculates the highest number in a vector of integers. Designed
 * for MULTI-THREADING.
 * @param param a integer vector full of numbers
 * @return nothing, but sets the global max variable to the highest value
 * in the vector
 */
void* calcMax(void *param) {
    std::vector<int>* integerList = (std::vector<int>*) param;

    max = integerList->at(0);
    for (int i = 1; i < integerList->size(); i++) {
        if(integerList->at(i) > max) {
            max = integerList->at(i);
        }
    }
    return nullptr;
}

int main(int argCount, char* argValue[]) {

    std::vector<int> integerList;

    // convert arguments to integer vector
    for (int i = 1; i < argCount; i++) {
        integerList.push_back(std::stoi(argValue[i]));
    }
    if (integerList.size() > 0) {
        pthread_t avg_tid, min_tid, max_tid;

        // create thread with default attributes
        pthread_create(&avg_tid, nullptr, calcAverage, &integerList);
        pthread_create(&min_tid, nullptr, calcMin, &integerList);
        pthread_create(&max_tid, nullptr, calcMax, &integerList);

        // wait for threads to finish
        pthread_join(avg_tid, nullptr);
        pthread_join(min_tid, nullptr);
        pthread_join(max_tid, nullptr);


        // output results
        cout << "The average is " << average << endl;
        cout << "The minimum is " << min << endl;
        cout << "The maximum is " << max << endl;
    } else {
        cout << "No parameters specified. Please execute program" << endl;
        cout << "and provide a list of integers as parameters." << endl;
    }


    return 0;
}
