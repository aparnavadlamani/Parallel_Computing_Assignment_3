//To compile: g++ -std=c++11 -pthread p5.cpp

#include<iostream>
#include<thread>
#include<vector>
#include<random>

using namespace std;

const int num_threads = 4;

int sum[num_threads];

void initialize_array(vector<int>& arr, int n)
{
    int lbound = 0;
    int ubound = 1000;
    uniform_int_distribution<int> uid(lbound, ubound);
    default_random_engine re;
    for(int i=0;i<n;i++)
    {
        arr.push_back(uid(re));
    }
}

void compute_sum(int tid, int n, vector<int>& d)
{
    int st = (n/num_threads)*tid;
    int en = (n/num_threads)*(tid+1);

    for(int i=st;i<en;i++)
    {
        sum[tid] += d[i];
    }
}

int main(int argc, char* argv[])
{
    vector<thread> threads;
    vector<int> data;
    
    int n = atoi(argv[1]);
    
    initialize_array(data, n);
    
    for(int i=0;i<num_threads;i++)
    {
        threads.push_back(thread(compute_sum, i, n, ref(data)));
    }
    for(auto &th:threads)
    {
        th.join();
    }
    int tsum = 0;
    for(int i=0;i<num_threads;i++)
    {
        tsum+=sum[i];
    }
    cout<<"Array sum is: "<<tsum<<endl;
    return 0;
}