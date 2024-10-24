//
// Created by mrtsstlrs on 24.10.2024.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"



inline int mpi_point_to_point()
{
	int ProcRank, ProcNum, i, j;
	int* a = new int[NMAX];
	int* b = new int[NMAX];
	double TotalSum = 0, ProcSum = 0;
	double st_time, end_time;
	double time_diff = 0, bcst_time;
	double stime;

	// MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	MPI_Status Status;

	// ���������� ������, �������� 0-�� ��������� �� ���� ���������
	if (ProcRank == 0) {
		printf(">>> MPI [point-to-point]\n");
		printf("1. DATA TYPE      : int\n");
		printf("2. VECTORS SIZE   : %d\n", NMAX);
		printf("3. VECTORS NUMBER : %d\n", 2);
		printf("4. THREADS NUMBER : [5, 10, 15]\n");
		printf("5. PARAMETER Q    : %d\n", Q);


		for (i = 0; i < NMAX; ++i) {
			a[i] = 1;
			b[i] = 2;
		}

		stime = sequental_time(a, b);
	}

	st_time = MPI_Wtime(); // ����� �������
	for (int cntr = 0; cntr < ITERS_NUMBER; ++cntr) {
		bcst_time = MPI_Wtime();
		MPI_Bcast(a, NMAX, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(b, NMAX, MPI_INT, 0, MPI_COMM_WORLD);
		time_diff += MPI_Wtime() - bcst_time;

		ProcSum = 0;

		int k = NMAX / ProcNum;
		int i1 = k * ProcRank;
		int i2 = k * (ProcRank + 1);

		if (ProcRank == ProcNum - 1) i2 = NMAX;
		for (i = i1; i < i2; i++) {
			for (j = 0; j < Q; ++j) {
				ProcSum = ProcSum + a[i] + b[i];
			}
		}
		ProcSum = ProcSum / Q;


		if (ProcRank == 0)
		{
			TotalSum = ProcSum;
			for (i = 1; i < ProcNum; i++)
			{
				MPI_Recv(&ProcSum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &Status);
				TotalSum = TotalSum + ProcSum;
			}
		}
		else {
			MPI_Send(&ProcSum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}

	end_time = MPI_Wtime(); // ��������� �������
	end_time = (end_time - st_time) / ITERS_NUMBER;

	if (ProcRank == 0)
	{
		printf("\nTotal Sum = %10.2f", TotalSum);
		printf("\nTIME OF WORK IS %f ", end_time);
		printf("\nACCELERATION W/I BCAST IS %f ", stime / end_time);
		printf("\nACCELERATION W/O BCAST IS %f ", stime / (end_time - time_diff / 20));
	}

	delete[] a;
	delete[] b;

	// MPI_Finalize();

	return 0;
}