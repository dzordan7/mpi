#include <stdio.h>
#include <mpi.h>

#define n 5

int main(int argc, char *argv[])
{

    int rank, a[n][n], i, j;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Datatype newtype;
    MPI_Type_vector(n, 1, n, MPI_INT, &newtype);
    MPI_Type_commit(&newtype);

    if (rank == 0)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                a[i][j] = i;
            }
        }

        printf("Matrica u procesu 0: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }

        MPI_Send(&a[0][0], 1, newtype, 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                a[i][j] = 0;
            }
        }

        printf("Matrica u procesu 1: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }

        MPI_Recv(&a[0][0], n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        printf("Rezultujuca matrica: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
