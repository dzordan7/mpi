#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{

    int mcol, irow, jcol, p;
    MPI_Comm row_comm, col_comm;
    int Iam, row_id, col_id;
    mcol = 2;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &Iam);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    irow = Iam / mcol;
    jcol = Iam / mcol;

    MPI_Comm_split(MPI_COMM_WORLD, irow, jcol, &row_comm);
    MPI_Comm_split(MPI_COMM_WORLD, jcol, irow, &col_comm);

    MPI_Comm_rank(row_comm, &row_id);
    MPI_Comm_rank(col_comm, &col_id);

    printf("%8d %8d %8d %8d %8d\n", Iam, irow, jcol, row_id, col_id);

    MPI_Finalize();
    return 0;
}