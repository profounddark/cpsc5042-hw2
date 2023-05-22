#include <iostream>
#include <pthread.h>




using std::cout, std::cin, std::endl;

int* initArray;
int* finalArray;

struct SortParameters {
    int first;
    int last;
};

void* sortArray(void *param) {
    SortParameters* sortParam = (SortParameters*) param;

    int first = sortParam->first;
    int last = sortParam->last;

    // laziest copy of the insertion sort ever
    int i = first + 1;
    while (i < last) {
        int j = i;
        while ((j > first) && (initArray[j - 1] > initArray[j])) {
            std::swap(initArray[j], initArray[j-1]);
            j--;
        }
        i++;
    }

}

/**
 * mergeArray merges two halves of the global initArray into finalArray.
 * @param param the size of the array
 * @return nothing, but finalArray is filled with the merged form of initArray
 */
void* mergeArray(void *param) {
    int size = *(int *) param;
    int midpoint = size / 2;

    int firstCount = 0;
    int secondCount = midpoint;

    int finalCount = 0;

    // go through each, merging along the way
    while ((firstCount < midpoint) && (secondCount < size)) {
        if (initArray[firstCount] < initArray[secondCount]) {
            finalArray[finalCount] = initArray[firstCount];
            firstCount++;
        } else {
            finalArray[finalCount] = initArray[secondCount];
            secondCount++;
        }
        finalCount++;
    }

    // if first half didn't finish, finish it
    for (; firstCount < midpoint; firstCount++) {
        finalArray[finalCount] = initArray[firstCount];
        finalCount++;
    }

    // if second half didn't finish, finish it
    for (; secondCount < size; secondCount++) {
        finalArray[finalCount] = initArray[secondCount];
        finalCount++;
    }


}


int main() {

    int size;
    cout << "Enter the number of elements to be sorted: ";
    cin >> size;
    initArray = new int[size];
    finalArray = new int[size];


    cout << "Enter the elements of the array: ";
    for (int i = 0; i < size; i++) {
        cin >> initArray[i];
    }
    int midpoint = size / 2;
    cout << "num= " << size << ", half size: " << midpoint << endl;



    pthread_t first_tid, second_tid, merge_tid;

    SortParameters firstParams = {0, midpoint};
    SortParameters secondParams = {midpoint, size};

    // create thread with default attributes
    pthread_create(&first_tid, nullptr, sortArray, &firstParams);
    pthread_create(&second_tid, nullptr, sortArray, &secondParams);
    // have to wait for the two individual sorts to finish
    pthread_join(first_tid, nullptr);
    pthread_join(second_tid, nullptr);

    // before merging!
    pthread_create(&merge_tid, nullptr, mergeArray, &size);

    // wait for merge
    pthread_join(merge_tid, nullptr);

    cout << "Sorted array:" << endl;
    for (int i = 0; i < size; i++) {
        cout << finalArray[i] << " ";
    }
    cout << endl;

    // delete dynamic memory
    delete[] initArray;
    delete[] finalArray;

    return 0;
}
