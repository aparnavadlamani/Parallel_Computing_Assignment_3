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
    int n, rank, p, num, num_p;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Status status;

    if(rank==0)
    {
        int st, temp;
        n = atoi(argv[1]);
        
        int* arr = new int[n];

        initialize_array(arr, n);

        /*for(int i=0;i<n;i++)
        {
            cout<<arr[i]<<" ";
        }
        cout<<endl;*/
        num = n/p;
        if(p>1)
        {
            for(int i=1;i<p-1;i++)
            {
                st = i*num;
                MPI_Send(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&arr[st], num, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            st = (p-1)*num;
            int rem = n-st;
            MPI_Send(&rem, 1, MPI_INT, p-1, 0, MPI_COMM_WORLD);
            MPI_Send(&arr[st], rem, MPI_INT, p-1, 0, MPI_COMM_WORLD);
        }
        int sum = 0;
        for(int i=0;i<num;i++)
        {
            sum+=arr[i];
        }
        for(int i=1;i<p;i++)
        {
            MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            sum+=temp;
        }
        cout<<"Array sum: "<<sum<<endl;
    }
    else
    {
        MPI_Recv(&num_p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    
        int arr1[num_p];

        MPI_Recv(&arr1, num_p, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        int partsum = 0;
        for(int i=0;i<num_p;i++)
        {
            partsum+=arr1[i];
        }
        MPI_Send(&partsum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}