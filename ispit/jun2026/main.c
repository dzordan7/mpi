#include <stdio.h>
#include <mpi.h>

#define m 5
#define k 3
#define n 4

int main(int argc, char *argv[])
{

    int rank, p, a[m][k], b[k][n], c[m][n], i, j;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    int local_b[k][n / p];
    int local_c[m][n / p];

    MPI_Status status;
    MPI_Datatype newtype, column, rez, resize;
    MPI_Type_vector(k, n / p, n, MPI_INT, &newtype);
    MPI_Type_commit(&newtype);
    MPI_Type_create_resized(newtype, 0, (n / p) * sizeof(int), &column);
    MPI_Type_commit(&column);

    MPI_Type_vector(m, n / p, n, MPI_INT, &rez);
    MPI_Type_commit(&rez);
    MPI_Type_create_resized(rez, 0, (n / p) * sizeof(int), &resize);
    MPI_Type_commit(&resize);

    if (rank == 0)
    {
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < k; j++)
            {
                a[i][j] = i + 1;
            }
        }

        printf("Matrica a: \n");
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < k; j++)
            {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }

        for (i = 0; i < k; i++)
        {
            for (j = 0; j < n; j++)
            {
                b[i][j] = m + i + 1;
            }
        }

        printf("Matrica b: \n");
        for (i = 0; i < k; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", b[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Bcast(&a[0][0], m * k, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {

        for (i = 0; i < k; i++)
        {
            for (j = 0; j < n / p; j++)
            {
                local_b[i][j] = b[i][j];
            }
        }

        for (i = 1; i < p; i++)
        {

            MPI_Send(&b[0][i * n / p], 1, column, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(&local_b[0][0], k * (n / p), MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n / p; j++)
        {
            local_c[i][j] = 0;

            for (int t = 0; t < k; t++)
            {
                local_c[i][j] += a[i][t] * local_b[t][j];
            }
        }
    }

    MPI_Gather(&local_c, m * (n / p), MPI_INT, &c, 1, resize, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {

        printf("Rezultujuca matrica: \n");
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", c[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Type_free(&newtype);
    MPI_Type_free(&column);
    MPI_Type_free(&rez);
    MPI_Type_free(&resize);

    MPI_Finalize();
    return 0;
}