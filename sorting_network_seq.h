#pragma once

#include <iostream>
#include <algorithm>
#include <cmath>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>

class SortingNetworkSeq
{
public:
    void sort(int arr[]);
    SortingNetworkSeq();
    explicit SortingNetworkSeq(int len);
    ~SortingNetworkSeq();

private:
    int *len_per_step;
    int step_count;
    int ***steps;
    int *arr;
};
