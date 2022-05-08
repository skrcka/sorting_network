#include <iostream>
#include <algorithm>
#include <algorithm>
#include <chrono>
#include <string.h>
#include <fstream>

#include "sorting_network.h"
#include "sorting_network_omp.h"
#include "sorting_network_seq.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::sort;
using namespace std::chrono;

int main()
{
    srand(time(0));
    ofstream f("results.csv");
    f << "Array count,Array size,Sorter,Time" << endl;
    for(int xd=0; xd < 20; xd++){
    for(int arrsize = 8; arrsize <= 256; arrsize+=8) {
        SortingNetwork *sorter = new SortingNetwork(arrsize);
        SortingNetworkOmp *ompsorter = new SortingNetworkOmp(arrsize);
        SortingNetworkSeq *seqsorter = new SortingNetworkSeq(arrsize);

        uint64_t time_sorter = 0;
        uint64_t time_openmp = 0;
        uint64_t time_sequential = 0;
        uint64_t time_stdsort = 0;
        for(int arrcount = 10; arrcount <= 100; arrcount+=10) {
            int arr[arrcount][arrsize];
            for (int i = 0; i < arrcount; i++)
            {
                for (int j = 0; j < arrsize; j++)
                {
                    arr[i][j] = (rand() % 100);
                }
            }

            for (int i = 0; i < arrcount; i++)
            {
                int arrcopy[arrsize];
                memcpy(arrcopy, arr[i], arrsize * sizeof(int));
                auto start = high_resolution_clock::now();
                sorter->sort(arrcopy);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                time_sorter += duration.count();
            }
            f << arrcount << ',' << arrsize << ",own sorting network," << time_sorter << endl;

            for (int i = 0; i < arrcount; i++)
            {
                int arrcopy[arrsize];
                memcpy(arrcopy, arr[i], arrsize * sizeof(int));
                auto start = high_resolution_clock::now();
                ompsorter->sort(arrcopy);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                time_openmp += duration.count();
            }
            f << arrcount << ',' << arrsize << ",openmp sorting network," << time_openmp << endl;

            for (int i = 0; i < arrcount; i++)
            {
                int arrcopy[arrsize];
                memcpy(arrcopy, arr[i], arrsize * sizeof(int));
                auto start = high_resolution_clock::now();
                seqsorter->sort(arrcopy);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                time_sequential += duration.count();
            }
            f << arrcount << ',' << arrsize << ",sequential sorting network," << time_sequential << endl;

            for (int i = 0; i < arrcount; i++)
            {
                int arrcopy[arrsize];
                memcpy(arrcopy, arr[i], arrsize * sizeof(int));
                auto start = high_resolution_clock::now();
                sort(arrcopy, arrcopy + arrsize);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                time_stdsort += duration.count();
            }
            f << arrcount << ',' << arrsize << ",std::sort," << time_stdsort << endl;
        }
        delete sorter;
        delete ompsorter;
        delete seqsorter;
    }
    }

    f.close();

    return 0;
}
