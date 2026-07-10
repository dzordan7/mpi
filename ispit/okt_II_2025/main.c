#include <stdio.h>
#include <mpi.h>

#define n 3
#define k 4
#define m 6

int main(int argc, char *argv[])
{
    int rank, p, a[n][k], b[k][m], c[n][m], i, j;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int local_b[k][m / p];
    int local_c[n][m / p];

    MPI_Datatype newtype, newResized;
    MPI_Type_vector(k, m / p, m, MPI_INT, &newtype);
    MPI_Type_create_resized(newtype, 0, (m / p) * sizeof(int), &newResized);
    MPI_Type_commit(&newResized);

    if (rank == 0)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < k; j++)
            {
                a[i][j] = i + 1;
            }
        }

        for (i = 0; i < k; i++)
        {
            for (j = 0; j < m; j++)
            {
                b[i][j] = n + i + 1;
            }
        }

        printf("Matrica A: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < k; j++)
            {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }

        printf("Matrica B: \n");
        for (i = 0; i < k; i++)
        {
            for (j = 0; j < m; j++)
            {
                printf("%d ", b[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Bcast(&a[0][0], n * k, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, 1, newResized, local_b, k * (m / p), MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {

        printf("loacl_b: \n");
        for (i = 0; i < k; i++)
        {
            for (j = 0; j < m / p; j++)
            {
                printf("%d ", local_b[i][j]);
            }
            printf("\n");
        }
    }

    // mnozenje

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m / p; j++)
        {
            local_c[i][j] = 0;

            for (int t = 0; t < k; t++)
            {
                local_c[i][j] += a[i][t] * local_b[t][j];
            }
        }
    }

    if (rank == 0)
    {

        printf("loacl_c: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m / p; j++)
            {
                printf("%d ", local_c[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Datatype rez, resizedRez;
    MPI_Type_vector(n, m / p, m, MPI_INT, &rez);
    MPI_Type_create_resized(rez, 0, (m / p) * sizeof(int), &resizedRez);
    MPI_Type_commit(&resizedRez);

    MPI_Gather(&local_c, n * (m / p), MPI_INT, c, 1, resizedRez, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {

        printf("Matrica C: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < m; j++)
            {
                printf("%d ", c[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Type_free(&newtype);
    MPI_Type_free(&newResized);
    MPI_Type_free(&rez);
    MPI_Type_free(&resizedRez);
    MPI_Finalize();
    return 0;
}