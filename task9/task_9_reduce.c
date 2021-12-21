#include <iostream>
#include <mpi.h>

using namespace std;

void my_sum(int* x, int* y, int* n, MPI_Datatype *dtype)
{
    for (int i = 0; i < *n; i++)
        y[i] += x[i];
}

int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	MPI_Op vec_sum;
    MPI_Op_create((MPI_User_function *)my_sum, 1, &vec_sum);

	int N = 1000000;
    int* arr = new int[N];
    for (int i = 0; i < N; i++)
        arr[i] = 1;

	double start_time, elapsed_time;
	start_time = MPI_Wtime();

    int* result = new int[N];

    MPI_Reduce(arr, result, N, MPI_INT, vec_sum, 0, MPI_COMM_WORLD);

    if (rank == 0)
	{
		elapsed_time = MPI_Wtime() - start_time;
		cout << "First 5 result elements: " << endl;
		for (int i = 0; i < 5; i++)
		    cout << result[i] << " ";
		cout << "\nExecution time: " << elapsed_time << " seconds" << endl;
	}
    MPI_Finalize();
    return 0;
 }