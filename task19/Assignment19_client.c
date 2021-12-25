#include <stdio.h>
#include <string.h>
#include "mpi.h"
int main(int argc, char **argv)
{
	int rank;
	int r[2];
	r[0] = 42;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Comm intercomm;
	MPI_Status status;
	strcpy(port_name, argv[1]);
	printf("Attempt to connect\n");
	MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
	printf("Server connection\n");
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Sendrecv(&r[0], 1, MPI_INT, 0, 1, &r[1], 1, MPI_INT, 0, 0, intercomm, &status);
	printf("Client sent value: %d\n", r[0]);
	printf("Client got value: %d\n", r[1]);
	MPI_Finalize();
	return 0;
}