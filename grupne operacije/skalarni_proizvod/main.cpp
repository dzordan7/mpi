#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>

#define n 6

int main(int argc, char *argv[])
{

    float a[n], b[n], dot, local_dot = 0;
    int i, n_bar, my_rank, p;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    n_bar = n / p;

    float *local_a = (float *)malloc(n_bar * sizeof(float));
    float *local_b = (float *)malloc(n_bar * sizeof(float));

    if (my_rank == 0)
    {
        for (i = 0; i < n; i++)
        {

            std::cout << "Unesi vrednosti za vektor a: " << std::endl;
            std::cin >> a[i];
        }
    }

    MPI_Scatter(a, n_bar, MPI_FLOAT, local_a, n_bar, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (my_rank == 0)
    {
        for (i = 0; i < n; i++)
        {

            std::cout << "Unesi vrednosti za vektor b: " << std::endl;
            std::cin >> b[i];
        }
    }
    MPI_Scatter(b, n_bar, MPI_FLOAT, local_b, n_bar, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (i = 0; i < n_bar; i++)
    {
        local_dot = local_dot + local_a[i] * local_b[i];
    }
    MPI_Reduce(&local_dot, &dot, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0)
    {
        std::cout << "Suma je " << dot;
    }

    MPI_Finalize();

    return 0;
}