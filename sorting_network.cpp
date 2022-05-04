#include "sorting_network.h"
#include <cstdio>

using std::cout;
using std::endl;
using std::floor;
using std::min;
using std::swap;
using std::thread;

void odd_even_8::thread_fn(int index)
{
    // cout << "started thread: " << index << endl;
    while (!killed)
    {
        if (run[index])
        {
            int step = this->step;
            if (index >= len_per_step[step])
            {
                run[index] = false;
                continue;
            }
            if (step == -1)
                continue;
            // printf("stage:%d; index: %d\n", step, index);
            // printf("nums: %d, %d\n", arr[steps[step][index][0]], arr[steps[step][index][1]]);
            if (arr[steps[step][index][0]] > arr[steps[step][index][1]])
            {
                // cout << step << " " << arr[steps[step][index][0]] << " and " << arr[steps[step][index][1]] << " index: " << index << endl;
                swap(arr[steps[step][index][0]], arr[steps[step][index][1]]);
            }
            // cout << "done: " << done.load() << endl;
            done++;
            // inc_done();
            run[index] = false;
        }
    }
}

void odd_even_8::reset()
{
    this->reset_run(false);
    this->step = -1;
    this->arr = nullptr;
    this->done = 0;
}

void odd_even_8::reset_run(bool value)
{
    for (int i = 0; i < 4; i++)
    {
        run[i] = value;
    }
}

void odd_even_8::kill()
{
    this->killed = true;
    for (int i = 0; i < sizeof(threads) / sizeof(threads[0]); i++)
    {
        this->threads[i].join();
    }
}

odd_even_8::odd_even_8()
{
    this->killed = false;
    this->reset();
    steps[0][0][0] = 0;
    steps[0][0][1] = 1;
    steps[0][1][0] = 2;
    steps[0][1][1] = 3;
    steps[0][2][0] = 4;
    steps[0][2][1] = 5;
    steps[0][3][0] = 6;
    steps[0][3][1] = 7;

    steps[1][0][0] = 0;
    steps[1][0][1] = 2;
    steps[1][1][0] = 1;
    steps[1][1][1] = 3;
    steps[1][2][0] = 4;
    steps[1][2][1] = 6;
    steps[1][3][0] = 5;
    steps[1][3][1] = 7;

    steps[2][0][0] = 1;
    steps[2][0][1] = 2;
    steps[2][1][0] = 5;
    steps[2][1][1] = 6;

    steps[3][0][0] = 0;
    steps[3][0][1] = 4;
    steps[3][1][0] = 1;
    steps[3][1][1] = 5;
    steps[3][2][0] = 2;
    steps[3][2][1] = 6;
    steps[3][3][0] = 3;
    steps[3][3][1] = 7;

    steps[4][0][0] = 2;
    steps[4][0][1] = 4;
    steps[4][1][0] = 3;
    steps[4][1][1] = 5;

    steps[5][0][0] = 1;
    steps[5][0][1] = 2;
    steps[5][1][0] = 3;
    steps[5][1][1] = 4;
    steps[5][2][0] = 5;
    steps[5][2][1] = 6;

    len_per_step[0] = 4;
    len_per_step[1] = 4;
    len_per_step[2] = 2;
    len_per_step[3] = 4;
    len_per_step[4] = 2;
    len_per_step[5] = 3;

    for (int i = 0; i < sizeof(threads) / sizeof(threads[0]); i++)
    {
        this->threads[i] = thread(&odd_even_8::thread_fn, this, i);
    }
}

void odd_even_8::sort(int *arr)
{
    this->arr = arr;
    this->step = 0;
    this->reset_run(true);
    while (step < 6)
    {
        if (done == len_per_step[step])
        {
            if (step == 5)
                break;
            step++;
            // printf("step: %d\n", step.load());
            done = 0;
            this->reset_run(true);
        }
    }
    reset();
}

void make_step(int arr[], int step[][2], int len)
{
    for (int i = 0; i < len; i++)
    {
        if (arr[step[i][0]] > arr[step[i][1]])
        {
            swap(arr[step[i][0]], arr[step[i][1]]);
        }
    }
}

void check_and_swap(int arr[], int index1, int index2)
{
    if (arr[index1] > arr[index2])
    {
        swap(arr[index1], arr[index2]);
    }
}

void make_step_parael(int arr[], int step[][2], int len)
{
    thread threads[len];
    for (int i = 0; i < len; i++)
    {
        threads[i] = thread(check_and_swap, arr, step[i][0], step[i][1]);
    }
    for (int i = 0; i < len; i++)
        threads[i].join();
}

void sort_odd_even8(int arr[], bool use_threads)
{
    int step1[4][2];
    step1[0][0] = 0;
    step1[0][1] = 1;
    step1[1][0] = 2;
    step1[1][1] = 3;
    step1[2][0] = 4;
    step1[2][1] = 5;
    step1[3][0] = 6;
    step1[3][1] = 7;
    if (use_threads)
        make_step_parael(arr, step1, 4);
    else
        make_step(arr, step1, 4);

    int step2[4][2];
    step2[0][0] = 0;
    step2[0][1] = 2;
    step2[1][0] = 1;
    step2[1][1] = 3;
    step2[2][0] = 4;
    step2[2][1] = 6;
    step2[3][0] = 5;
    step2[3][1] = 7;
    if (use_threads)
        make_step_parael(arr, step2, 4);
    else
        make_step(arr, step2, 4);

    int step3[2][2];
    step3[0][0] = 1;
    step3[0][1] = 2;
    step3[1][0] = 5;
    step3[1][1] = 6;
    if (use_threads)
        make_step_parael(arr, step3, 2);
    else
        make_step(arr, step3, 2);

    int step4[4][2];
    step4[0][0] = 0;
    step4[0][1] = 4;
    step4[1][0] = 1;
    step4[1][1] = 5;
    step4[2][0] = 2;
    step4[2][1] = 6;
    step4[3][0] = 3;
    step4[3][1] = 7;
    if (use_threads)
        make_step_parael(arr, step4, 4);
    else
        make_step(arr, step4, 4);

    int step5[2][2];
    step5[0][0] = 2;
    step5[0][1] = 4;
    step5[1][0] = 3;
    step5[1][1] = 5;
    if (use_threads)
        make_step_parael(arr, step5, 2);
    else
        make_step(arr, step5, 2);

    int step6[2][2];
    step6[0][0] = 2;
    step6[0][1] = 4;
    step6[1][0] = 3;
    step6[1][1] = 5;
    if (use_threads)
        make_step_parael(arr, step6, 2);
    else
        make_step(arr, step6, 2);

    int step7[3][2];
    step7[0][0] = 1;
    step7[0][1] = 2;
    step7[1][0] = 3;
    step7[1][1] = 4;
    step7[2][0] = 5;
    step7[2][1] = 6;
    if (use_threads)
        make_step_parael(arr, step7, 3);
    else
        make_step(arr, step7, 3);
}
