// Distributions of data
// Two common distributions are the block and cyclic. The block distribution is often used when the computational load is distributed homogeneously over a regular data structure, such as a Cartesian grid. It assigns blocks of size r of the global vector to the processes. For block distribution, the mapping m(p, i) is defined as:

//     m↦—>(floor(m/L), m mod L)

// where L = ceiling(M/P). The cyclic distribution (also known as the wrapped or scattered decomposition) is commonly used to improve load balance when the computational load is distributed inhomogeneously over a regular data structure. The cyclic distribution assigns consecutive entries of the global vector to successive processes. For cyclic distribution, the mapping m(p, i) is defined as:

//     m↦—>(m mod P, floor(m/P))

// The block-cyclic distribution is a generalization of the block and cyclic distributions, in which blocks of r consecutive data objects are distributed cyclically over the p processes. This can be described by a mapping of the global index m (0 ≤ m < M) of a data object to an index triplet (p,b,i), where p (0 ≤ p < P) specifies the process to which the data object is mapped, b is the block number in process p, and i is the location in the block. For block-cyclic distribution, the mapping m(p, b, i) is defined as:

//     m↦—>(floor((m mod T)/r), floor(m/T), m mod r)

// where T = rP. (It should be noted that this reverts to the cyclic distribution when r = 1 and a block distribution when r = L.) The inverse mapping to a global index (p, b, i)m is defined by:

//     (p, b, i)Br+i = pr+bT+i

// where B = p+bP is the global block number.
// The block-cyclic distribution can reproduce most of the data distributions commonly used in linear algebra computations on parallel computers. 

#include<iostream>
#include<mpi.h>

using namespace std;

int main(int argc, char* argv[])
{
    int rank, p, n, c, num;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    n = atoi(argv[1]);
    c = atoi(argv[2]);

    int* arr;
    int k;

    switch(c){
        case 0://Block distribution
            num = n/p;
            if(rank==p-1)
            {
                num+=(n%p);
            }
            arr = new int[num];
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
        break;
        case 1://Cyclic Distribution
            num = n/p;
            if(n%p>rank)
            {
                num+=1;
            }
            arr = new int[num];
            k = rank;
            for(int i=0;i<num;i++)
            {
                arr[i] = k;
                k+=p;
            }
            break;
        case 2://Block cyclic distribution
            int r = 2;
            int b = n/r;
            int x = n%r;
            num = (b/p)*r;
            if(b%p>rank)
            {
                num+=r;
            }
            if(rank==b%p)
            {
                num+=x;
            }
            k = r*rank;
            for(int i=0;i<num;i++)
            {
                if(i%2==0)
                {
                    arr[i] = k;
                }
                else
                {
                    arr[i] = k+1;
                    k+=r*p;
                }
            }
            break;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for(int i=0;i<num;i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
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