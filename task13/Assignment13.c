#include <cstdlib>
#include "mpi.h"
#include <time.h>
#include <iostream>

using namespace std;
double** malloc_array(int n)
{
	double** matrix = new double*[n];
	for (int i = 0; i < n; i++)
		matrix[i] = new double[n];
	return matrix;
}

// Memory free function
void free_array(double** matrix, int n)
{
	for (int i = 0; i < n; i++)
		delete[] matrix[i];
	delete[] matrix;
}

void rand_init_matrix(double** matrix, int n)
{
	srand(time(NULL));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = double(rand()) / RAND_MAX;
}

// Matrix zeroing function
void zero_init_matrix(double** matrix, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = 0.0;
}

int main(int argc, char **argv)
{
    int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//matrix initialization by each process
    int N = 500;
    double** A, **B, **C;
    A = malloc_array(N);
	B = malloc_array(N);
	C = malloc_array(N);
	rand_init_matrix(A, N);
	rand_init_matrix(B, N);
	zero_init_matrix(C, N);

	MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization
// start timing for each process
    double start_time, elapsed_time;
	start_time = MPI_Wtime();
// matrix multiplication
    for (int i = 0; i < N; i++)
		for (int k = 0; k < N; k++)
			for (int j = 0; j < N; j++)
				C[i][j] += A[i][k] * B[k][j];
// end of timing
    elapsed_time = MPI_Wtime() - start_time;
// output the execution time of matrix multiplication at each process
    cout << "Process: " << rank << ", execution time: " << elapsed_time << " seconds\n";

    free_array(A, N);
	free_array(B, N);
	free_array(C, N);
	MPI_Finalize();
}