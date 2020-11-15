#include<iostream>
#include<random>
#include<omp.h>

using namespace std;

//A and B will be the input Matrices - maximum size here 1000
double matrixA[1000][1000];
double matrixB[1000][1000];
double transposeB[1000][1000];

//this matrix will be the multiplication of Matrix A and B
double matrixC[2000][2000];

void transpose(int n, int p) 
{
    int i,j;
    for(i=0; i<n; i++) 
    {
        for(j=0; j<p; j++) 
        {
            transposeB[i][j] = matrixB[j][i];
        }
    }
}
void initialize_matrix(int m, int n, int p)
{
	int lbound = 0;
    int ubound = 1000;
    uniform_real_distribution<double> urd(lbound, ubound);
    default_random_engine re;

	for (int i = 0; i < m; i++)
    {
		for (int j = 0; j < n; j++)
        {
			matrixA[i][j] = urd(re);
		}
	}
    for (int i = 0; i < n; i++)
    {
		for (int j = 0; j < p; j++)
        {
			matrixB[i][j] = urd(re);
		}
	}
}

void matrix_multiplication(int m, int n, int p)
{
	int i, j, k;
	//get the transpose of matrixB
    transpose(n, p);
	
	#pragma omp parallel for shared(matrixA, matrixB, matrixC) private(i, j, k)
	for (i = 0; i < m; i++){
		for (j = 0; j < n; j++){
			double sum = 0;
			for (k = 0; k < p; k++){
				sum += matrixA[i][k] * transposeB[j][k];
			}		
			matrixC[i][j] = sum;
		}
	}
}

int main(int argc, char* argv[]) 
{
    int m, n, p, num_threads;
    m = atoi(argv[1]);
    n = atoi(argv[2]);
    p = atoi(argv[3]);
    num_threads = atoi(argv[4]);
    omp_set_num_threads(num_threads);
	initialize_matrix(m, n, p);
    matrix_multiplication(m, n, p);
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<p;j++)
        {
            cout<<matrixC[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}

