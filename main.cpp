#include <iostream>
#include <algorithm>
#include <algorithm>
#include <chrono>

#include "sorting_network.h"

using std::cout;
using std::endl;
using namespace std::chrono;

#define ARRCOUNT 1
#define ARRSIZE 8

int main()
{
    srand(time(0));
    int arr[ARRCOUNT][ARRSIZE];
    for (int i = 0; i < ARRCOUNT; i++)
    {
        for (int j = 0; j < ARRSIZE; j++)
        {
            arr[i][j] = (rand() % 100);
        }
    }

    SortingNetwork *sorter = new SortingNetwork(ARRSIZE);

    for (int i = 0; i < ARRCOUNT; i++)
    {
        auto start = high_resolution_clock::now();
        sorter->sort(arr[i]);
        // sort_odd_even8(arr[i], true); // norm: 1400, par: 2349900
        auto stop = high_resolution_clock::now(); // time: 4300

        auto duration = duration_cast<nanoseconds>(stop - start);

        for (int j = 0; j < ARRSIZE; j++)
        {
            cout << arr[i][j] << ',';
        }
        cout << endl;

        printf("%d time: %ld\n", i, duration.count());
    }

    delete sorter;

    return 0;
}
