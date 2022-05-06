#pragma once

#include <iostream>
#include <algorithm>
#include <cmath>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>

class SortingNetworkOmp
{
public:
    void sort(int arr[]);
    SortingNetworkOmp();
    explicit SortingNetworkOmp(int len);
    ~SortingNetworkOmp();

private:
    int *len_per_step;
    int step_count;
    int ***steps;
    int *arr;
};
