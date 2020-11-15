//To compile: gcc -fopenmp filename.c

#include<iostream>
#include<omp.h>
#include<random>

using namespace std;

void initialize_array(unsigned long int* arr, int n)
{
    int lbound = 0;
    int ubound = 1000;
    uniform_int_distribution<int> uid(lbound, ubound);
    default_random_engine re;
    for(int i=0;i<n;i++)
    {
        arr[i] = uid(re);
    }
}
int main(int argc , char **argv)
{
    unsigned long int *arr, sum = 0;
    unsigned long int arr_size, i, num_threads;
	
    num_threads = atoi(argv[1]);
    arr_size = atoi(argv[2]); 

	arr = new unsigned long int[arr_size];
	
	initialize_array(arr, arr_size);

    omp_set_num_threads(num_threads);
 
    #pragma omp parallel for reduction(+:sum)
    for(unsigned long i=0;i<arr_size;i++)
    {
        sum = sum+arr[i];
    }

	free(arr);
	cout<<"Array sum is "<<sum<<endl;
    return 0;
}
