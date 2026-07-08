#include <stdio.h>
#include <mpi.h>

#define n 5

int main(int argc, char *argv[])
{

    int rank, size, i, j, a[n][n], b[n][n];
    int displacments[n], block_lengths[n];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    MPI_Datatype newtype;

    for (i = 0; i < n; i++)
    {
        block_lengths[i] = n - i;      // duzine blokova
        displacments[i] = (n + 1) * i; // pomeraji pocetka blokova u odnosu na pocetak
    }
    MPI_Type_indexed(n, block_lengths, displacments, MPI_INT, &newtype);
    MPI_Type_commit(&newtype);

    if (rank == 0)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                a[i][j] = i + j;
            }
        }

        printf("Matrica A: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }

        MPI_Send(a, 1, newtype, 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                b[i][j] = 0;
            }
        }

        printf("Matrica B: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", b[i][j]);
            }
            printf("\n");
        }

        MPI_Recv(b, 1, newtype, 0, 0, MPI_COMM_WORLD, &status);

        printf("Rezultujuca matrica B: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", b[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}