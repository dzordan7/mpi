#include <stdio.h>
#include <mpi.h>

#define n 3
#define s 4
#define k 6

int main(int argc, char *argv[])
{

    int rank, p, a[n][s], b[s][k], c[n][k], i, j;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int local_b[s * k / p];
    int local_prod[s];
    int prod[s];
    int local_c[n * k / p];

    MPI_Datatype singlecol,
        multicol, resized, resizedSingle;
    MPI_Type_vector(s, 1, k, MPI_INT, &singlecol);
    MPI_Type_commit(&singlecol);
    MPI_Type_create_resized(singlecol, 0, sizeof(int), &resizedSingle);
    MPI_Type_vector(k / p, 1, p, resizedSingle, &multicol);
    MPI_Type_commit(&multicol);
    MPI_Type_create_resized(multicol, 0, sizeof(int), &resized);
    MPI_Type_commit(&resized);

    if (rank == 0)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < s; j++)
            {
                a[i][j] = i + 1;
            }
        }

        for (i = 0; i < s; i++)
        {
            for (j = 0; j < k; j++)
            {
                b[i][j] = n + i + 1;
            }
        }

        printf("Matirca A: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < s; j++)
            {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }

        printf("Matirca B: \n");
        for (i = 0; i < s; i++)
        {
            for (j = 0; j < k; j++)
            {
                printf("%d ", b[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Bcast(&a[0][0], n * s, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, 1, resized, local_b, s * k / p, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < s; i++)
    {
        local_prod[i] = 1;
        for (j = 0; j < k / p; j++)
        {
            local_prod[i] *= local_b[j * s + i];
        }
    }

    MPI_Reduce(local_prod, &prod, s, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (i = 0; i < s * k / p; i++)
        {
            printf("%d ", local_b[i]);
        }
        printf("\n");

        for (i = 0; i < s; i++)
        {
            printf("Proizvod %d vrste je: %d \n", i + 1, prod[i]);
        }
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < k / p; j++)
        {
            local_c[i * k / p + j] = 0;

            for (int t = 0; t < s; t++)
            {
                local_c[i * k / p + j] += a[i][t] * local_b[j * s + t];
            }
        }
    }

    MPI_Datatype newtype, resizedNew;
    MPI_Type_vector(n * k / p, 1, p, MPI_INT, &newtype);
    MPI_Type_create_resized(newtype, 0, sizeof(int), &resizedNew);
    MPI_Type_commit(&resizedNew);

    MPI_Gather(local_c, n * k / p, MPI_INT, c, 1, resizedNew, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {

        printf("Matirca C: \n");
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < k; j++)
            {
                printf("%d ", c[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Type_free(&singlecol);
    MPI_Type_free(&multicol);
    MPI_Type_free(&resized);
    MPI_Type_free(&resizedSingle);
    MPI_Type_free(&newtype);
    MPI_Type_free(&resizedNew);
    // MPI_Type_free(&singleColC);
    // MPI_Type_free(&resizedSingleColC);
    // MPI_Type_free(&multiColC);
    // MPI_Type_free(&resizedMultiColC);

    MPI_Finalize();

    return 0;
}