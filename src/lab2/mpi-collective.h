#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#include <iostream>
#include "utils.h"



inline int mpi_collective()
{
    double TotalSum, ProcSum = 0.0;

    int* a = new int[NMAX];
    int* b = new int[NMAX];

    int ProcRank, ProcNum, i, j, k;
    MPI_Status Status;

    double st_time, end_time;
    double time_diff = 0, bcst_time;
    double stime;

    // MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0)
    {
        printf(">>> MPI [collective]\n");
        printf("1. DATA TYPE      : int\n");
        printf("2. VECTORS SIZE   : %d\n", NMAX);
        printf("3. VECTORS NUMBER : %d\n", 2);
        printf("4. THREADS NUMBER : [5, 10, 15]\n");
        printf("5. PARAMETER Q    : %d\n", Q);

        for (int i = 0; i < NMAX; ++i) {
            a[i] = 1;
            b[i] = 2;
        }

        stime = sequental_time(a, b);
    }

    st_time = MPI_Wtime();
    for (int cntr = 0; cntr < ITERS_NUMBER; ++cntr) {
        bcst_time = MPI_Wtime();
        MPI_Bcast(a, NMAX, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(b, NMAX, MPI_INT, 0, MPI_COMM_WORLD);
        time_diff += MPI_Wtime() - bcst_time;

        ProcSum = 0.0;
        int k = NMAX / ProcNum;
        int start_idx = k * ProcRank;
        int end_idx = (ProcRank == ProcNum - 1) ? NMAX : k * (ProcRank + 1);

        for (int i = start_idx; i < end_idx; ++i) {
            for (int j = 0; j < Q; ++j) {
                ProcSum = ProcSum + a[i] + b[i];
            }
        }

        MPI_Reduce(&ProcSum, &TotalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    end_time = (end_time - st_time) / ITERS_NUMBER;
    TotalSum /= Q;

    if (ProcRank == 0)
    {
        printf("\nTotal Sum = %10.2f", TotalSum);
        printf("\nTIME OF WORK IS %f ", end_time);
        printf("\nACCELERATION W/I BCAST IS %f ", stime / end_time);
        printf("\nACCELERATION W/O BCAST IS %f ", stime / (end_time - time_diff / 20));
    }

    // MPI_Finalize();

    delete[] a;
    delete[] b;

    return 0;
}