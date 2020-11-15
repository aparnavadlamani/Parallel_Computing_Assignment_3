#include<iostream>
#include<mpi.h>
#include<random>

using namespace std;

void initialize_array(int* arr, int n)
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
int main(int argc, char* argv[])
{
    int rank, p;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Status status;

    int n = atoi(argv[1]);

    int* arr = new int[n];
    int arr1[n];
    initialize_array(arr, n);

    if(rank==0)
    {
        MPI_Send(&arr, n, MPI_INT, (rank+1)%p, 0, MPI_COMM_WORLD);
        MPI_Recv(&arr1, n, MPI_INT, p-1, 0, MPI_COMM_WORLD, &status);
        
    }
    else
    {
        MPI_Send(&arr, n, MPI_INT, (rank+1)%p, 0, MPI_COMM_WORLD);
        MPI_Recv(&arr1, n, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
    }
    for(int i=0;i<p;i++)
    {
        if(rank==i)
        {
            cout<<"Process "<<rank<<" received ";
            int sender = status.MPI_SOURCE;
            for(int i=0;i<n;i++)
            {
                cout<<arr1[i]<<" ";
            }
            cout<<" from process"<<sender<<endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Finalize();

    return 0;
}