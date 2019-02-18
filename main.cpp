#include <thread>
#include <vector>
#include "PairwiseStitching.h"
#include <iostream>

using namespace std;

int main()
{
    cout << "set affinity before going on\n";
    system("pause");

    const int num_threads = thread::hardware_concurrency();// created thread number = processor number to achieve highest speed

    vector<thread> stitching_threads;
    for (int i = 0; i < num_threads; i++)
        stitching_threads.push_back(thread(PairwiseStitching));
    for (int i = 0; i < num_threads; i++)
        stitching_threads[i].join();

    system("pause");
    return 0;
}