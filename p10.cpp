#include<iostream>
#include<random>
#include<mpi.h>

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
    int rank, p, n, m;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    n = atoi(argv[1]);
    m = atoi(argv[2]);
    //cout<<n<<" "<<m<<endl;

    int* arr = new int[n];
    //initialize_array(arr, n);
    for(int i=0;i<n;i++)
    {
        arr[i] = rank;
    }
    int x=0, y;
    for(int i=0;i<m;i++)
    {
        if(rank==x)
        {
            initialize_array(&y, 1);
            y = y%p;
        }
        MPI_Bcast(&y, 1, MPI_INT, x, MPI_COMM_WORLD);
        if(x==y)
            continue;
        if(rank==x)
        {
            MPI_Send(&arr, n, MPI_INT, y, 0, MPI_COMM_WORLD);
        }
        if(rank==y)
        {
            MPI_Recv(&arr, n, MPI_INT, x, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        x = y;
        MPI_Barrier(MPI_COMM_WORLD);
    }
    for(int i=0;i<p;i++)
    {
        if(rank==i)
        {
            cout<<"Process "<<rank<<" elements: ";
            for(int j=0;j<n;j++)
            {
                cout<<arr[j]<<" ";
            }
            cout<<endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}