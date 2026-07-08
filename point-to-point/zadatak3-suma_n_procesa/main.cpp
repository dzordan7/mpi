#include "stdio.h"
#include "iostream"
#include "mpi.h"

// ako bi vrednosti procesa bili raziciti od ranka morala bi da se doda neka nova
// promeljiva value

int main(int argc, char *argv[])
{

    int rank, size, value, sum = 0;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        value = 1;
        MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        if (rank < size - 1)
        {

            MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
            sum = rank + 1 + value;
            MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
        else
        {
            MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
            sum = rank + 1 + value;
            std::cout << "Suma za prvih " << size << " procesa je " << sum << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}