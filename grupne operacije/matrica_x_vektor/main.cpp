#include <iostream>
#include <stdio.h>
#include <mpi.h>

#define n 3

int main(int argc, char *argv[])
{

    int rank, size, matrix[n][n], b[n], c[n], i, j, local_a[n], local_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // int *local_a = (int *)malloc(n * sizeof(int));
    // int *local_b = (int *)malloc(n * sizeof(int));

    if (rank == 0)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                std::cout << "Unesi [" << i + 1 << "] [" << j + 1 << "] element matrice: ";
                std::cin >> matrix[i][j];
            }
        }

        for (i = 0; i < n; i++)
        {
            std::cout << "Unesi [" << i + 1 << "] element niza: ";
            std::cin >> b[i];
        }
    }

    MPI_Scatter(&matrix[0][0], n, MPI_INT, local_a, n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, n, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < n; i++)
    {
        local_sum += local_a[i] * b[i];
    }

    MPI_Gather(&local_sum, 1, MPI_INT, &c[0], 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        std::cout << "Rezultujuci vektor: ";
        for (i = 0; i < n; i++)
        {
            std::cout << c[i] << " ";
        }
    }

    MPI_Finalize();
    return 0;
}
