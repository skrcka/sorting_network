#pragma once

#include <iostream>
#include <algorithm>
#include <cmath>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>

void sort_odd_even8(int arr[], bool use_threads);

class SortingNetwork
{
public:
    void sort(int arr[]);
    SortingNetwork();
    ~SortingNetwork();

private:
    void reset();
    void reset_run(bool value);
    std::thread* threads;
    void thread_fn(int index);
    int* len_per_step;
    std::atomic<bool> killed;
    bool run[4];
    int step_count;
    int thread_count;
    int*** steps;
    int *arr;
    std::atomic<int> step;
    std::atomic<int> done;
};