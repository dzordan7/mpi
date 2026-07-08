#include <stdio.h>
#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[])
{

    struct
    {
        double value;
        int rank;

    } in, out;

    int rank, root;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    in.value = rank + 1;
    in.rank = rank;
    root = 5;
    MPI_Reduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MAXLOC, root, MPI_COMM_WORLD);
    if (rank == root)
    {
        std::cout << "Root proces [" << rank << "] ,max je: " << out.value << ", rank je:" << out.rank << std::endl;
    }
    MPI_Reduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MINLOC, root, MPI_COMM_WORLD);
    if (rank == root)
    {
        std::cout << "Root proces [" << rank << "] ,min je: " << out.value << ", rank je:" << out.rank << std::endl;
    }
    MPI_Finalize();
    return 0;
}