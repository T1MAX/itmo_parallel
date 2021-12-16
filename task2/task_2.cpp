#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

using namespace std;

// The function of allocating memory for a two-dimensional array
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

// The function of initializing a matrix with random numbers from [0,1]
void rand_init_matrix(double** matrix, int n)
{
	srand(time(NULL));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = rand() / RAND_MAX;
}

// Matrix zeroing function
void zero_init_matrix(double** matrix, int n)
{
	srand(time(NULL));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = 0.0;
}

void ijk_multiplication(double** matA, double** matB, double** res_mat, int n)
{
    for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				res_mat[i][j] += matA[i][k] * matB[k][j];
}

void jki_multiplication(double** matA, double** matB, double** res_mat, int n)
{
    for (int j = 0; j < n; j++)
		for (int k = 0; k < n; k++)
			for (int i = 0; i < n; i++)
				res_mat[i][j] += matA[i][k] * matB[k][j];
}

void ikj_multiplication(double** matA, double** matB, double** res_mat, int n)
{
    for (int i = 0; i < n; i++)
		for (int k = 0; k < n; k++)
			for (int j = 0; j < n; j++)
				res_mat[i][j] += matA[i][k] * matB[k][j];
}

bool is_arrays_equal(double** first, double** second, int n)
{
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            if(first[i][j] != second[i][j])
                return false;
    return true;
}

int main(int argc, char** argv)
{
	const int N = atoi(argv[1]);
	double** A, **B, **C, **C_test;

// Memory allocation for matrices A, B, C
	A = malloc_array(N);
	B = malloc_array(N);
	C = malloc_array(N);
	C_test = malloc_array(N);

// Initialization of matrices
	rand_init_matrix(A, N);
	rand_init_matrix(B, N);
	zero_init_matrix(C, N);
	zero_init_matrix(C_test, N);
	double t;

// Matrix multiplication with cycle order ijk
    cout << "ijk multiplication" << endl;

	t = omp_get_wtime();
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				C_test[i][j] += A[i][k] * B[k][j];
	double t_ijk = omp_get_wtime() - t;

    for(int n_threads = 1; n_threads <= 10; n_threads++)
    {
        t = omp_get_wtime();
        #pragma omp parallel for num_threads(n_threads)
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    for (int k = 0; k < N; k++)
                        C[i][j] += A[i][k] * B[k][j];
        double t_ijk_parallel = omp_get_wtime() - t;

        if(!is_arrays_equal(C, C_test, N))
            cout << "not equal, ";
        cout << "Multiplication time with " << n_threads << " threads: " << t_ijk_parallel << " seconds, efficiency: " << t_ijk / t_ijk_parallel << endl;
    }
	cout << "------------" <<  endl;

// Matrix multiplication with cycle order jki
    cout << "jki multiplication" << endl;

	zero_init_matrix(C, N);
	t = omp_get_wtime();
	for (int j = 0; j < N; j++)
		for (int k = 0; k < N; k++)
			for (int i = 0; i < N; i++)
				C[i][j] += A[i][k] * B[k][j];
	double t_jki = omp_get_wtime() - t;

    for(int n_threads = 1; n_threads <= 10; n_threads++)
    {
        zero_init_matrix(C, N);
        t = omp_get_wtime();
        #pragma omp parallel for num_threads(n_threads)
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                    for (int i = 0; i < N; i++)
                        C[i][j] += A[i][k] * B[k][j];
        double t_jki_parallel = omp_get_wtime() - t;

        if(!is_arrays_equal(C, C_test, N))
            cout << "not equal, ";
        cout << "Multiplication time with " << n_threads << " threads: " << t_jki_parallel << " seconds, efficiency: " << t_jki / t_jki_parallel << endl;
    }
	cout << "------------" <<  endl;

// Matrix multiplication with cycle order ikj
    cout << "ikj multiplication" << endl;

	zero_init_matrix(C, N);
	t = omp_get_wtime();
	for (int i = 0; i < N; i++)
		for (int k = 0; k < N; k++)
			for (int j = 0; j < N; j++)
				C[i][j] += A[i][k] * B[k][j];
	double t_ikj = omp_get_wtime() - t;

    for(int n_threads = 1; n_threads <= 10; n_threads++)
    {
        zero_init_matrix(C, N);
        t = omp_get_wtime();
        #pragma omp parallel for num_threads(n_threads)
            for (int i = 0; i < N; i++)
                for (int k = 0; k < N; k++)
                    for (int j = 0; j < N; j++)
                        C[i][j] += A[i][k] * B[k][j];
        double t_ikj_parallel = omp_get_wtime() - t;

        if(!is_arrays_equal(C, C_test, N))
            cout << "not equal";
        cout << "Multiplication time with " << n_threads << " threads: " << t_ikj_parallel << " seconds, efficiency: " << t_ikj / t_ikj_parallel << endl;
    }
    cout << "------------" <<  endl;

// Freeing memory occupied by matrices A, B, C
	free_array(A, N);
	free_array(B, N);
	free_array(C, N);
    free_array(C_test, N);
	return 0;
}