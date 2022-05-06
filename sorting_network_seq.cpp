#include "sorting_network_seq.h"
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

SortingNetworkSeq::~SortingNetworkSeq()
{
    for (int i = 0; i < step_count; i++)
    {
        for (int j = 0; j < len_per_step[i]; j++)
            delete[] steps[i][j];
        delete[] steps[i];
    }
    delete[] len_per_step;
    delete[] steps;
}

SortingNetworkSeq::SortingNetworkSeq(int len)
{
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
}

SortingNetworkSeq::SortingNetworkSeq()
{
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
        steps[i] = new int *[len_per_step[i]];
        for (int j = 0; j < len_per_step[i]; j++)
        {
            steps[i][j] = new int[2];
        }
    }
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
}

void SortingNetworkSeq::sort(int *arr)
{
    this->arr = arr;
    for(int i=0; i < step_count; i++){
        for(int j=0; j < len_per_step[i]; j++){
            if(arr[steps[i][j][0]] > arr[steps[i][j][1]])
                swap(arr[steps[i][j][0]], arr[steps[i][j][1]]);
        }
    }
}
