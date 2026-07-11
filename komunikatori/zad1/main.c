#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

    int p, color, rank, rankc, size, key;
    MPI_Comm newcomm;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    color = rank % 2;
    key = 7;

    MPI_Comm_split(MPI_COMM_WORLD, color, key, &newcomm);
    MPI_Comm_size(newcomm, &size);
    MPI_Comm_rank(newcomm, &rankc);

    printf("Moj rank je: %d, moj kljuc je: %d, moj rankc je: %d, moj color je: %d,moj size je: %d \n", rank, key, rankc, color, size);

    MPI_Finalize();

    return 0;
}