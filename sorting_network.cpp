#include "sorting_network.h"
#include <vector>
#include <cstdio>

using std::count;
using std::cout;
using std::endl;
using std::floor;
using std::make_pair;
using std::max_element;
using std::min;
using std::pair;
using std::swap;
using std::thread;
using std::vector;

#define MAXTHREAD 12

void SortingNetwork::thread_fn(int index)
{
    while (!killed)
    {
        if (run[index] == 1)
        {
            run[index] = 2;
            if (index >= len_per_step[step])
            {
                run[index] = 0;
                continue;
            }
            // fprintf(stderr, "stage: %d; index: %d\n", step.load(), index);
            if (arr[steps[step][index][0]] > arr[steps[step][index][1]])
            {
                swap(arr[steps[step][index][0]], arr[steps[step][index][1]]);
            }
            run[index] = 0;
        }
    }
    return;
}

void SortingNetwork::reset()
{
    this->reset_run(0);
    this->step = -1;
    this->arr = nullptr;
}

void SortingNetwork::reset_run(int value)
{
    for (int i = 0; i < thread_count; i++)
    {
        run[i] = value;
    }
}

SortingNetwork::~SortingNetwork()
{
    this->killed = true;
    for (int i = 0; i < thread_count; i++)
    {
        this->threads[i].join();
    }
    delete[] threads;
    delete[] run;
    for (int i = 0; i < step_count; i++)
    {
        for (int j = 0; j < len_per_step[i]; j++)
            delete[] steps[i][j];
        delete[] steps[i];
    }
    delete[] len_per_step;
    delete[] steps;
}

SortingNetwork::SortingNetwork(int len)
{
    this->killed = false;
    vector<int> vec1;
    vector<int> vec2;

    for (int p = 1; p < len; p += p)
        for (int k = p; k > 0; k /= 2)
            for (int j = k % p; j <= len - 1 - k; j += (k + k))
                for (int i = 0; i <= min(k - 1, len - j - k - 1); i++)
                    if (floor((j + i) / (p + p)) == floor((j + i + k) / (p + p)))
                    {
                        vec1.push_back(j + i);
                        vec2.push_back(j + i + k);
                    }

    int size = vec1.size();
    vector<int> visited;
    vector<int> lens;

    for (int i = 0; i < size; i++)
    {
        if (count(visited.begin(), visited.end(), vec1[i]) || count(visited.begin(), visited.end(), vec2[i]))
        {
            int vis = visited.size() / 2;
            lens.push_back(vis);

            visited.clear();
        }

        visited.push_back(vec1[i]);
        visited.push_back(vec2[i]);

        if (i == size - 1)
        {
            int vis = visited.size() / 2;
            lens.push_back(vis);

            visited.clear();
        }
    }
    step_count = lens.size();
    len_per_step = new int[step_count];
    for (int i = 0; i < step_count; i++)
    {
        len_per_step[i] = lens[i];
    }
    thread_count = *std::max_element(begin(lens), end(lens));

    threads = new thread[thread_count];
    run = new int[thread_count];
    steps = new int **[step_count];

    int index = 0;
    for (int i = 0; i < step_count; i++)
    {
        steps[i] = new int *[lens[i]];
        for (int j = 0; j < lens[i]; j++)
        {
            steps[i][j] = new int[2];
            steps[i][j][0] = vec1[index];
            steps[i][j][1] = vec2[index];
            // cout << "steps[" << i << "][" << j << "]" << "[0]: " << steps[i][j][0] << endl;
            // cout << "steps[" << i << "][" << j << "]" << "[1]: " << steps[i][j][1] << endl;
            index++;
        }
    }

    this->reset();

    for (int i = 0; i < thread_count; i++)
    {
        this->threads[i] = thread(&SortingNetwork::thread_fn, this, i);
    }
}

SortingNetwork::SortingNetwork()
{
    thread_count = 4;
    threads = new thread[thread_count];
    run = new int[thread_count];
    step_count = 6;
    steps = new int **[step_count];
    len_per_step = new int[step_count];
    len_per_step[0] = 4;
    len_per_step[1] = 4;
    len_per_step[2] = 2;
    len_per_step[3] = 4;
    len_per_step[4] = 2;
    len_per_step[5] = 3;
    for (int i = 0; i < step_count; i++)
    {
        steps[i] = new int *[thread_count];
        for (int j = 0; j < len_per_step[i]; j++)
        {
            steps[i][j] = new int[2];
        }
    }
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

    for (int i = 0; i < thread_count; i++)
    {
        this->threads[i] = thread(&SortingNetwork::thread_fn, this, i);
    }
}

int SortingNetwork::check_run()
{
    for (int i = 0; i < thread_count; i++)
    {
        if (run[i] == 1)
            return 1;
        if (run[i] == 2)
            return 2;
    }
    return 0;
}

void SortingNetwork::sort(int *arr)
{
    this->arr = arr;
    this->step = 0;
    this->reset_run(1);
    while (step < step_count)
    {
        if (!check_run())
        {
            if (step == step_count - 1)
                break;
            step++;
            this->reset_run(1);
        }
    }
    reset();
}

void make_step(int *arr, int step[][2], int len)
{
    for (int i = 0; i < len; i++)
    {
        if (arr[step[i][0]] > arr[step[i][1]])
        {
            swap(arr[step[i][0]], arr[step[i][1]]);
        }
    }
}

void check_and_swap(int *arr, int index1, int index2)
{
    if (arr[index1] > arr[index2])
    {
        swap(arr[index1], arr[index2]);
    }
}

void make_step_parael(int *arr, int step[][2], int len)
{
    thread threads[len];
    for (int i = 0; i < len; i++)
    {
        threads[i] = thread(check_and_swap, arr, step[i][0], step[i][1]);
    }
    for (int i = 0; i < len; i++)
        threads[i].join();
}

void sort_odd_even8(int *arr, bool use_threads)
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
