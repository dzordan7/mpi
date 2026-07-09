#include <stdio.h>
#include <mpi.h>

#define m 2
#define n 3
#define k 4

// Mnozimo matrice A(m x n) X B(n x k) - master salje po kolonu od A i po vrstu od B

int main(int argc, char *argv[])
{

    int rank, a[m][n], b[n][k], c[m][k], local_c[m][k], niz_a[m], niz_b[k], i, j, p;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Datatype vector, column;

    MPI_Type_vector(m, 1, n, MPI_INT, &vector);
    MPI_Type_commit(&vector);
    MPI_Type_create_resized(vector, 0, 1 * sizeof(int), &column);
    MPI_Type_commit(&column);

    if (rank == 0)
    {
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
            {
                a[i][j] = i + j;
            }
        }

        printf("Matrica A: \n");
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
            {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < k; j++)
            {
                b[i][j] = 1 + j - 1;
            }
        }

        printf("Matrica B: \n");

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < k; j++)
            {
                printf("%d ", b[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Scatter(a, 1, column, niz_a, m, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, k, MPI_INT, niz_b, k, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < k; j++)
        {
            local_c[i][j] = niz_a[i] * niz_b[j];
        }
    }

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < k; j++)
        {
            c[i][j] = 0;
        }
    }

    MPI_Reduce(&local_c[0][0], &c[0][0], m * k, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {

        printf("Rezultujuca matrica: \n");
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < k; j++)
            {
                printf("%d ", c[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}