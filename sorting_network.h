#pragma once

#include <iostream>
#include <algorithm>
#include <cmath>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>

void sort_odd_even8(int arr[], bool use_threads);

class SortingNetwork
{
public:
    void sort(int arr[]);
    explicit SortingNetwork(int len);
    ~SortingNetwork();

private:
    void reset();
    void thread_fn();
    int *len_per_step;
    int step_count;
    int thread_count;
    int ***steps;
    int *arr;
    int step;

    std::queue<int> jobs;
    std::mutex jobs_mutex;
    std::condition_variable jobs_cond;
    std::vector<std::thread> threads;

    std::mutex done_mutex;
    std::atomic<int> done;
    std::atomic<bool> kill;
};
