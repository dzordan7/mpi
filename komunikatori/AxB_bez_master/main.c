#include <stdio.h>
#include <mpi.h>

#define n 3

int main(int argc, char *argv[])
{

    int rank, size, m_rank;

    int a[n][n], b[n][n], rez[n][n], vrsta[n], kolona[n], tmp[n][n];
    int root = 0;

    int no[1] = {0};
    int br = 1;
    int i, j;

    MPI_Status status;
    MPI_Datatype vector;
    MPI_Group mat, world;
    MPI_Comm comm;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Type_vector(n, 1, n, MPI_INT, &vector);
    MPI_Type_commit(&vector);

    MPI_Comm_group(MPI_COMM_WORLD, &world);
    MPI_Group_excl(world, br, no, &mat);
    MPI_Comm_create(MPI_COMM_WORLD, mat, &comm);
    MPI_Group_rank(mat, &m_rank);

    if (rank == root)
    {

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                a[i][j] = 3 * i + j + 1;
                b[i][j] = i + 1;
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

        printf("Matrica B: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", b[i][j]);
            }

            printf("\n");
        }

        for (i = 0; i < n; i++)
        {
            MPI_Send(&a[0][i], 1, vector, i + 1, 33, MPI_COMM_WORLD);
            MPI_Send(&b[i][0], n, MPI_INT, i + 1, 32, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(&kolona[0], n, MPI_INT, root, 33, MPI_COMM_WORLD, &status);
        MPI_Recv(&vrsta[0], n, MPI_INT, root, 32, MPI_COMM_WORLD, &status);

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                tmp[i][j] = kolona[i] * vrsta[j];
            }
        }

        MPI_Reduce(&tmp, &rez, n * n, MPI_INT, MPI_SUM, 0, comm);
    }

    if (m_rank == 0)
    {
        printf("Rezultujuca matrica: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", rez[i][j]);
            }

            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}