#include "iostream"
#include "mpi.h"

int main(int argc, char *argv[])
{

    int myrank, size;
    MPI_Status status;
    int x, y;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (myrank == 0)
    {
        x = 3;
        MPI_Recv(&y, 1, MPI_INT, 1, 19, MPI_COMM_WORLD, &status);
        MPI_Send(&x, 1, MPI_INT, 1, 17, MPI_COMM_WORLD);
    }
    else if (myrank == 1)
    {
        x = 5;
        MPI_Send(&x, 1, MPI_INT, 0, 19, MPI_COMM_WORLD);
        MPI_Recv(&y, 1, MPI_INT, 0, 17, MPI_COMM_WORLD, &status);
    }

    std::cout << "Proc: " << myrank << ", y= " << y << std::endl;
    MPI_Finalize();

    return 0;
}
