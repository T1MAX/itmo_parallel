#include <iostream>
#include "mpi.h"
#include <ctime>
#include <time.h>

using namespace std;
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    double time_start = MPI_Wtime();
	int rank, n, i, message;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &n);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
	{
	    bool has_adj = false;
	    int* arr = new int[n];

		for (i = 1; i < n; i++) {
			MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE,
			         MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			if (message % 2 == 0 && i > 1)
			    for (int j = 0; j < i - 1; j++)
			        if (message + 2 == arr[j] || message - 2 == arr[j])
			            has_adj = true;

			arr[i - 1] = message;
		}
		cout << "The sequence ";
        for (int i = 0; i < n - 1; i++)
            cout << arr[i] << " ";
		if (has_adj)
		    cout << "contains at least two adjacent even numbers";
		else cout << "doesn't contain two adjacent even numbers";
		cout << endl;
		double time_finish = MPI_Wtime();
		cout << "Execution time is " << time_finish - time_start << " seconds" << endl;
	}
	else
	{
	    srand(time(NULL) + rank);
        int size = 20;
        int number = rand() / (RAND_MAX / size);
	    MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}