#include <iostream>
#include <algorithm>
#include <algorithm>
#include <chrono>
#include <string.h>

#include "sorting_network.h"
#include "sorting_network_omp.h"
#include "sorting_network_seq.h"

using std::cout;
using std::endl;
using namespace std::chrono;

#define ARRCOUNT 1000
#define ARRSIZE 32

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
    SortingNetworkOmp *ompsorter = new SortingNetworkOmp(ARRSIZE);
    SortingNetworkSeq *seqsorter = new SortingNetworkSeq(ARRSIZE);

    for (int i = 0; i < ARRCOUNT; i++)
    {
        int arrcopy[ARRSIZE];
        memcpy(arrcopy, arr[i], ARRSIZE * sizeof(int));

        auto start = high_resolution_clock::now();
        //sorter->sort(arr[i]);
        seqsorter->sort(arrcopy);
        // sort_odd_even8(arr[i], true);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        printf("arr(%d) sequential time: %ld\n", i, duration.count());

        memcpy(arrcopy, arr[i], ARRSIZE * sizeof(int));
        start = high_resolution_clock::now();
        ompsorter->sort(arrcopy);
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        printf("arr(%d) openmp time: %ld\n", i, duration.count());

        memcpy(arrcopy, arr[i], ARRSIZE * sizeof(int));
        start = high_resolution_clock::now();
        sorter->sort(arr[i]);
        ompsorter->sort(arrcopy);
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        printf("arr(%d) own paralel time: %ld\n", i, duration.count());
    }

    //delete sorter;
    delete ompsorter;
    delete seqsorter;

    return 0;
}
