#include <iostream>
#include <mpi.h>

using namespace std;
int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	int N = 1000000;
    int* arr = new int[N];
    for (int i = 0; i < N; i++)
        arr[i] = 1;

	double start_time, elapsed_time;
	start_time = MPI_Wtime();

    int* temp = new int[N];
	for (int i = 2; i <= size; i *= 2)
	{
	    // on each iteration the left process of each pair receives message from the right process
		if (rank % i == 0)
		{
		    MPI_Recv(temp, N, MPI_INT, rank + i / 2, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		    for (int j = 0; j < N; j++)
		        arr[j] += temp[j];
		}
		else
		{
			MPI_Send(arr, N, MPI_INT, rank - i / 2, 0, MPI_COMM_WORLD);
			break; // after sending a message to its neighbor, the process may die (it did its job)
		}
	}

	if (rank == 0)
	{
		elapsed_time = MPI_Wtime() - start_time;
		cout << "First 5 result elements: " << endl;
		for (int i = 0; i < 5; i++)
		    cout << arr[i] << " ";
		cout << "\nExecution time: " << elapsed_time << " seconds" << endl;
	}

	// Finalize MPI
	MPI_Finalize();
	return 0;
}