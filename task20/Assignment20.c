#include <stdio.h>
#include "mpi.h"
#define BUFSIZE 100

void create_txt()
{
    FILE* fp = fopen("file.txt" ,"a");
    fprintf(fp, "Specific text\n");
    fclose(fp);
}


int main(int argc, char **argv)
{
    create_txt();
	int bufsize, num, sum;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_File fh;
	char buf[BUFSIZE];
	MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	sum = 0;
	do {
		MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
		MPI_Get_count(&status, MPI_CHAR, &num);
		printf("buf=%s\n", buf);
		sum += num;
	} while (num >= BUFSIZE);
	MPI_File_close(&fh);
	MPI_File_delete("file.txt", MPI_INFO_NULL);
	printf("The number of read symbols: %d\n", sum);
	//print the number of read symbols sum from the file
	MPI_Finalize();
}