#include <stdio.h>
#include "mpi.h"

void create_txt()
{
    FILE* fp = fopen("file1.txt" ,"a");
    for (int i = 0; i < 10; i++)
        fprintf(fp, "Specific line %d", i);
    fclose(fp);
}

int main(int argc, char **argv)
{
    create_txt();
	int rank;
	MPI_Init(&argc, &argv);
	MPI_File fh;
	char buf[15];
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_File_open(MPI_COMM_WORLD, "file1.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, rank * 15, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	MPI_File_read_all(fh, buf, 15, MPI_CHAR, MPI_STATUS_IGNORE);
	printf("process %d, buf=%s\n", rank, buf);
	MPI_File_close(&fh);
	MPI_File_delete("file1.txt", MPI_INFO_NULL);
	MPI_Finalize();
}