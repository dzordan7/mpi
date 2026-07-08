#include <stdio.h>
#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[])
{

    int rank, source, result, root;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    root = 7;
    source = rank + 1;
    MPI_Reduce(&source, &result, 1, MPI_INT, MPI_PROD, root, MPI_COMM_WORLD); // svi se mnoze i upisuju u promenjivu result procesa sa rankom 7
    if (rank == root)
    {

        std::cout << "Proces " << rank << ",proizvod " << result << std::endl;
    }

    MPI_Finalize();
    return 0;
}