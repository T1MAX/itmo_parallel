#include <mpi.h>
#include <iostream>

using namespace std;

const int N = 1000000;

int dot_product(int *x, int *y, int n) {
    int prod = 0;
    for (int i = 0; i < n; i++)
        prod += x[i] * y[i];
    return prod;
}

int main(int argc, char *argv[])
{
    int i, rank, size, result;
    int* x = new int[N];
    int* y = new int[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
        for (int i = 0; i < N; i++)
        {
            x[i] = 1;
            y[i] = 1;
        }
    int local_N = N / size;
    int* local_x = new int[local_N];
    int* local_y = new int[local_N];
    MPI_Scatter(x, local_N, MPI_INT, local_x, local_N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(y, local_N, MPI_INT, local_y, local_N, MPI_INT, 0, MPI_COMM_WORLD);

    int local_prod = dot_product(local_x, local_y, local_N);
    MPI_Reduce(&local_prod, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) cout << "Dot product = " << result << endl;
    MPI_Finalize();
    return 0;
}