#include <iostream>
#include <algorithm>
#include <cmath>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>

void sort_odd_even8(int arr[], bool use_threads);

class odd_even_8
{
public:
    void sort(int arr[]);
    odd_even_8();
    void kill();

private:
    void reset();
    void reset_run(bool value);
    std::thread threads[4];
    void thread_fn(int index);
    int len_per_step[6];
    bool killed;
    bool run[4];
    int steps[6][4][2];
    int *arr;
    std::atomic<int> step;
    std::atomic<int> done;
};