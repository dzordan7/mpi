#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[])
{

    // broj procesa mora da ima celobrojni koren,procesi su organizovani kao matrica qxq

    int rank, p, q, rows, cols, color, diagonal_rank, diagonal_size, i, j, local_prod = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    MPI_Comm diagonal_comm;

    q = (int)sqrt((double)p);
    int a[q][q];

    if (rank == 0)
    {

        for (i = 0; i < q; i++)
        {
            for (j = 0; j < q; j++)
            {
                a[i][j] = i + j + 1;
            }
        }

        printf("Matrica A: \n");
        for (i = 0; i < q; i++)
        {
            for (j = 0; j < q; j++)
            {
                printf("%2d ", a[i][j]);
            }
            printf("\n");
        }
    }

    rows = rank / q;
    cols = rank % q;

    if (rows == cols)
        color = 1;
    else
        color = MPI_UNDEFINED;

    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &diagonal_comm);

    if (diagonal_comm != MPI_COMM_NULL)
    {

        MPI_Comm_rank(diagonal_comm, &diagonal_rank);
        MPI_Comm_size(diagonal_comm, &diagonal_size);

        int local_col[q];

        MPI_Datatype col, col_resized;
        MPI_Type_vector(q, 1, q, MPI_INT, &col);
        MPI_Type_create_resized(col, 0, sizeof(int), &col_resized);
        MPI_Type_commit(&col_resized);

        MPI_Scatter(a, 1, col_resized, local_col, q, MPI_INT, 0, diagonal_comm);

        for (i = 0; i < q; i++)
        {
            local_prod *= local_col[i];
        }

        // for (i = 0; i < diagonal_size; i++)
        // {
        //     if (diagonal_rank == i)
        //     {

        //         printf("Proces sa globalnim rankom %d,i dijagonalnim rankom:%d ,proizvod kolone: %d\n", rank, diagonal_rank, local_prod);
        //     }
        // }

        printf("Proces sa globalnim rankom %d,i dijagonalnim rankom:%d ,proizvod kolone: %d\n", rank, diagonal_rank, local_prod);
    }

    MPI_Finalize();
    return 0;
}