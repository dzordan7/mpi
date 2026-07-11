#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define name_length 20
#define zaposleni_po_procesu 2

typedef struct
{
    int maticni;
    char ime[name_length];
    char prezime[name_length];
    double plata;

} Zaposleni;

int main(int argc, char *argv[])
{

    int rank, p, q, cols, rows, color;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    MPI_Comm lower_comm;

    q = (int)sqrt((double)p);

    rows = rank / q;
    cols = rank % q;

    if (rows > cols)
        color = 1;
    else
        color = MPI_UNDEFINED;

    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &lower_comm);

    if (lower_comm != MPI_COMM_NULL)
    {

        int lower_rank, lower_size, total_zaposleni, i;
        Zaposleni local_zaposleni[zaposleni_po_procesu];
        Zaposleni *zaposleni = NULL;

        MPI_Datatype zaposleni_tip;

        MPI_Comm_rank(lower_comm, &lower_rank);
        MPI_Comm_size(lower_comm, &lower_size);

        int block_length[4] = {1, name_length, name_length, 1};
        MPI_Aint displacement[4];
        displacement[0] = offsetof(Zaposleni, maticni);
        displacement[1] = offsetof(Zaposleni, ime);
        displacement[2] = offsetof(Zaposleni, prezime);
        displacement[3] = offsetof(Zaposleni, plata);
        MPI_Datatype types[4] = {MPI_INT, MPI_CHAR, MPI_CHAR, MPI_DOUBLE};

        MPI_Type_create_struct(4, block_length, displacement, types, &zaposleni_tip);
        MPI_Type_commit(&zaposleni_tip);

        total_zaposleni = lower_size * zaposleni_po_procesu;

        if (lower_rank == 0)
        {
            zaposleni = malloc(total_zaposleni * sizeof(Zaposleni));

            for (i = 0; i < total_zaposleni; i++)
            {

                zaposleni[i].maticni = 1000 + i;
                snprintf(zaposleni[i].ime, name_length, "Ime %d", i + 1);
                snprintf(zaposleni[i].prezime, name_length, "Prezime %d", i + 1);
                zaposleni[i].plata = 5000 + i * 100;
            }
        }

        MPI_Scatter(zaposleni, zaposleni_po_procesu, zaposleni_tip, local_zaposleni, zaposleni_po_procesu, zaposleni_tip, 0, lower_comm);

        printf("Globalni rank: %d , lower rank: %d\n", rank, lower_rank);

        for (i = 0; i < zaposleni_po_procesu; i++)
        {
            printf("Maticni broj: %d, ime: %s, prezime: %s, plata: %.2f\n", local_zaposleni[i].maticni, local_zaposleni[i].ime, local_zaposleni[i].prezime, local_zaposleni[i].plata);
        }
    }

    MPI_Finalize();
    return 0;
}