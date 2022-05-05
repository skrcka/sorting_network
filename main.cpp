#include <iostream>
#include <algorithm>
#include <algorithm>
#include <chrono>
#include <unistd.h>

#include "sorting_network.h"

using std::cout;
using std::endl;
using namespace std::chrono;

int main()
{
    srand(time(0));
    int arr[256][8];
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            arr[i][j] = (rand() % 10);
        }
    }

    SortingNetwork *sorter = new SortingNetwork();

    sleep(1);

    for (int i = 0; i < 256; i++)
    {
        auto start = high_resolution_clock::now();
        sorter->sort(arr[i]);
        // sort_odd_even8(arr, false); // norm: 1400, par: 2349900
        auto stop = high_resolution_clock::now(); // time: 4300

        auto duration = duration_cast<nanoseconds>(stop - start);

        /*
        for(int j=0; j < 8; j++){
            cout << arr[i][j] << ',';
        }
        */
        printf("%d time: %ld\n", i, duration.count());
    }

    printf("before delete\n");
    delete sorter;
    printf("delete\n");


    return 0;
}