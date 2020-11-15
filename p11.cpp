// First all nodes (except the one with rank 0, the root node) send a notification that they have reached the barrier to the root node
// After that they block awaiting notification from the root
// The root node implements the other side of the communication. 
// First it blocks until it received n-1 notifications (one from every node in the group, except himself, since he is inside the barrier call already)
// Once all notifications have arrived, it sends out the messages that every node is waiting for, signalling that everyone has reached the barrier, after which it leaves the barrier call itself:
// So the communication pattern is first an n:1 from all nodes to the root and then a 1:n from the root back to all nodes.

#include<iostream>
#include<mpi.h>
#include<vector>
#include<thread>

using namespace std;

const int nthreads = 4;
int token = 0;
int count = 0;

pthread_mutex_t lock; 

void iittp_barrier(int rank,int p,int tid)
{
    if(tid==0)
    {
        if(rank==0)
        {
            MPI_Send(&token,1,MPI_INT,1,1,MPI_COMM_WORLD);
            MPI_Recv(&token,1,MPI_INT,p-1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }
        else 
        {
            MPI_Recv(&token,1,MPI_INT,rank-1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            MPI_Send(&token,1,MPI_INT,(rank+1)%p,1,MPI_COMM_WORLD);
        }
        MPI_Bcast(&token,1,MPI_INT,0,MPI_COMM_WORLD);
    }
    
    pthread_mutex_lock(&lock);
    count++;
    pthread_mutex_unlock(&lock);
    while((count%nthreads)!=0);
}

void run_thread(int tid, int rank, int p)
{
    cout<<rank<<":"<<tid<<endl;
    iittp_barrier(rank, p, tid);
}

int main(int argc, char* argv[])
{
    int rank, p;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    cout<<"Rank:"<<rank<<endl;
    vector<thread> threads;
    for(int i=0;i<nthreads;i++)
    {
        threads.push_back(thread(run_thread,i,rank,p));
    }
    for(auto &th:threads)
    {
        th.join();
    }
    cout<<"Rank:"<<rank<<endl;
    MPI_Finalize();
    return 0;
}