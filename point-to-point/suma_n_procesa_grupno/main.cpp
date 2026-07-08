#include "iostream"
#include "stdio.h"
#include "mpi.h"
#include "math.h"

int main(int argc, char *argv[])
{

    int rank, size, sum, source_tag = 0, destination_tag = 0;
    int noofLevels, level, source, destination, value, nextLevel;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    sum = rank;

    noofLevels = (int)(log((double)size) / log(2.0)); // broj nivoa po dubinu
    for (int i = 0; i < noofLevels; i++)
    {
        level = (int)(pow((double)2, (double)i)); // 2^i je udaljenost izmedju procesa koji komuniciraju po nivou

        if ((rank % level) == 0)
        {
            nextLevel = (int)(pow((double)2, (double)(i + 1))); // odredjuje ko salje ko prima
            if ((rank % nextLevel) == 0)
            {
                source = rank + level;
                MPI_Recv(&value, 1, MPI_INT, source, source_tag, MPI_COMM_WORLD, &status);
                sum += value;
            }
            else
            {
                destination = rank - level;
                MPI_Send(&sum, 1, MPI_INT, destination, destination_tag, MPI_COMM_WORLD);
            }
        }
    }
    if (rank == 0)
    {
        std::cout << "Suma prvih " << size << " brojeva je: " << sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}