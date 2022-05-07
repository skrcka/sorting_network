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

    for(int arrsize = 4; arrsize <= 256; arrsize*=2) {
        SortingNetwork *sorter = new SortingNetwork(arrsize);
        SortingNetworkOmp *ompsorter = new SortingNetworkOmp(arrsize);
        SortingNetworkSeq *seqsorter = new SortingNetworkSeq(arrsize);

        for(int arrcount = 10; arrcount <= 1000; arrcount+=10) {
            int arr[arrcount][arrsize];
            for (int i = 0; i < arrcount; i++)
            {
                for (int j = 0; j < arrsize; j++)
                {
                    arr[i][j] = (rand() % 100);
                }
            }

            uint64_t time = 0;
            for (int i = 0; i < arrcount; i++)
            {
                int arrcopy[arrsize];
                memcpy(arrcopy, arr[i], arrsize * sizeof(int));
                auto start = high_resolution_clock::now();
                sorter->sort(arrcopy);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                time += duration.count();
            }
            f << arrcount << ',' << arrsize << ",own sorting network," << time << endl;

            time = 0;
            for (int i = 0; i < arrcount; i++)
            {
                int arrcopy[arrsize];
                memcpy(arrcopy, arr[i], arrsize * sizeof(int));
                auto start = high_resolution_clock::now();
                ompsorter->sort(arrcopy);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                time += duration.count();
            }
            f << arrcount << ',' << arrsize << ",openmp sorting network," << time << endl;

            time = 0;
            for (int i = 0; i < arrcount; i++)
            {
                int arrcopy[arrsize];
                memcpy(arrcopy, arr[i], arrsize * sizeof(int));
                auto start = high_resolution_clock::now();
                seqsorter->sort(arrcopy);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                time += duration.count();
            }
            f << arrcount << ',' << arrsize << ",sequential sorting network," << time << endl;

            time = 0;
            for (int i = 0; i < arrcount; i++)
            {
                int arrcopy[arrsize];
                memcpy(arrcopy, arr[i], arrsize * sizeof(int));
                auto start = high_resolution_clock::now();
                sort(arrcopy, arrcopy + arrsize);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                time += duration.count();
            }
            f << arrcount << ',' << arrsize << ",std::sort," << time << endl;
        }
        delete sorter;
        delete ompsorter;
        delete seqsorter;
    }

    f.close();

    return 0;
}
