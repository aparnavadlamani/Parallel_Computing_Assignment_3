#include<iostream>
#include<mpi.h>

using namespace std;

int main(int argc, char* argv[])
{
    int rank, p, n;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    n = atoi(argv[1]);

    int num = n/p;
    if(rank==p-1)
    {
        num+=(n%p);
    }
    int* arr = new int[num];
    for(int i=0;i<num;i++)
    {
        if(rank==p-1)
        {
            arr[i] = (num-n%p)*rank+i;
        }
        else
        {
            arr[i] = num*rank+i;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    int sum = 0;
    int global_sum = 0;
    for(int i=0;i<num;i++)
    {
        sum+=arr[i];
    }
    MPI_Reduce(&sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank==0)
    {
        cout<<"Array sum is: "<<global_sum<<endl;
    }
    MPI_Finalize();
    return 0;
}