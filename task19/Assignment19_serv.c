#include <stdio.h>
#include "mpi.h"
int main(int argc, char **argv)
{
	int r[2];
	r[0] = 25;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Status status;
	MPI_Comm intercomm;
	MPI_Open_port(MPI_INFO_NULL, port_name);
	printf("portname: %s\nWaiting for the client ...\n", port_name);
	MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
	printf("Client connected\n");
	MPI_Sendrecv(&r[0], 1, MPI_INT, 0, 0, &r[1], 1, MPI_INT, 0, 1, intercomm, &status);
	printf("Server sent value: %d\n", r[0]);
	printf("Server got value: %d\n", r[1]);
	MPI_Comm_free(&intercomm);
	MPI_Close_port(port_name);
	MPI_Finalize();
	return 0;
}