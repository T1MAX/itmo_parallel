#include <iostream>
#include <stdlib.h>
#include <mpi.h>

using namespace std;
int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	long int N = 1;
	while (N < 10000000)
	{
		int* x = new int[N];
		for (int i = 0; i < N; i++)
			x[i] = 1;

		int loop_count = 10;

		double start_time, elapsed_time;
		start_time = MPI_Wtime();

		for (int i = 0; i < loop_count; i++)
		{
			if (rank == 0)
			{
				MPI_Send(x, N, MPI_INT, 1, 1, MPI_COMM_WORLD);
				MPI_Recv(x, N, MPI_INT, 1, 2, MPI_COMM_WORLD, &status);
			}
			else if(rank == 1)
			{
				MPI_Recv(x, N, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
				MPI_Send(x, N, MPI_INT, 0, 2, MPI_COMM_WORLD);
			}
		}
		elapsed_time = MPI_Wtime() - start_time;

        int L = N * sizeof(int);
        double bandwidth = 2.0 * loop_count * L / elapsed_time / 1024 / 1024;
        double latency = elapsed_time / (2.0 * loop_count);

		if(rank == 0) cout << "Length: " << L << " bytes, bandwidth: " << bandwidth << " MB/s, latency: " << latency << endl;
		free(x);

		N *= 10;
	}

	MPI_Finalize();

	return 0;
}