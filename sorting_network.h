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
    void reset_run(int value);
    std::thread* threads;
    void thread_fn(int index);
    int* len_per_step;
    bool killed;
    int* run;
    int step_count;
    int thread_count;
    int*** steps;
    int *arr;
    int check_run();
    std::atomic<int> step;
};
