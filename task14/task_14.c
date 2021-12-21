#include <iostream>
#include <time.h>
#include <cstdlib>
#include "mpi.h"
#define n 1000

using namespace std;
void my_max(void *a, void *b, int *l, MPI_Datatype *type)
{
	for (int i = 0; i < *l; i++)
	    ((int*)b)[i] = max(((int*)a)[i], ((int*)b)[i]);
}

int main(int argc, char **argv)
{
	int rank, size, i;
	int a[n];
	int b[n];
	int b_test[n];
	MPI_Init(&argc, &argv);
	MPI_Op op;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	srand(time(NULL) + rank);
	for (i = 0; i < n; i++) {
	    a[i] = rand() / (RAND_MAX / 1000);
	}
	printf("process %d a[0] = %d\n", rank, a[0]);
	MPI_Op_create(&my_max, 1, &op);
	MPI_Reduce(a, b, n, MPI_INT, op , 0, MPI_COMM_WORLD);
	MPI_Op_free(&op);

	MPI_Reduce(a, b_test, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank == 0) cout << "MPI_MAX result: " << b_test[0] << ", my result: " << b[0] << endl;
	MPI_Finalize();
	return 0;
}