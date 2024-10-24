//
// Created by mrtsstlrs on 24.10.2024.
//
#include <omp.h>
#include <cstdio>
#include <time.h>
#include <chrono>


// ���������� � �������������� ��������� reduction
inline double reduction_time(int* a, int* b) {
    double sum = 0, total_time = 0;
    int i, j;

    for (int cntr = 0; cntr < ITERS_NUMBER; ++cntr) {
        double start_time = omp_get_wtime();  // ����� �������

#pragma omp parallel for reduction(+:sum) shared(a, b) private(i, j)
        for (i = 0; i < NMAX; ++i) {
            for (j = 0; j < Q; ++j) {
                sum = sum + a[i] + b[i];
            }
        }

        double end_time = omp_get_wtime();  // ��������� �������
        total_time = total_time + (end_time - start_time);
    }

    sum = sum / (ITERS_NUMBER * Q);
    double time_of_work = total_time / ITERS_NUMBER;

    return time_of_work;
}

// ���������� � �������������� ��������� atomic
inline double atomic_time(int* a, int* b) {
    double sum = 0, total_time = 0;
    int i, j;

    for (int cntr = 0; cntr < ITERS_NUMBER; ++cntr) {
        double start_time = omp_get_wtime();  // ����� �������

#pragma omp parallel for shared(a, b) private(i, j)
        for (i = 0; i < NMAX; ++i) {
            double local_sum = 0;
            for (j = 0; j < Q; ++j) {
                local_sum = local_sum + a[i] + b[i];
            }
#pragma omp atomic
            sum += local_sum;
        }

        double end_time = omp_get_wtime();  // ��������� �������
        total_time = total_time + (end_time - start_time);
    }

    sum = sum / (ITERS_NUMBER * Q);
    double time_of_work = total_time / ITERS_NUMBER;

    return time_of_work;
}

// ���������� � �������������� ��������� critical
inline double critical_time(int* a, int* b) {
    double sum = 0, total_time = 0;
    int i, j;

    for (int cntr = 0; cntr < ITERS_NUMBER; ++cntr) {
        double start_time = omp_get_wtime();  // ����� �������

#pragma omp parallel for shared(a, b) private(i, j)
        for (i = 0; i < NMAX; ++i) {
            double local_sum = 0;
            for (j = 0; j < Q; ++j) {
                local_sum = local_sum + a[i] + b[i];
            }
#pragma omp critical
            {
                sum = sum + local_sum;
            }
        }

        double end_time = omp_get_wtime();  // ��������� �������
        total_time = total_time + (end_time - start_time);
    }

    sum = sum / (ITERS_NUMBER * Q);
    double time_of_work = total_time / ITERS_NUMBER;

    return time_of_work;
}

inline double init_time() {
    double total_time = 0, start_time, end_time;
    for (int count = 0; count < ITERS_NUMBER; count++) {
        start_time = clock();
#pragma omp parallel
        {}
        end_time = clock();
        total_time += (end_time - start_time) / CLOCKS_PER_SEC;
    }
    return total_time / ITERS_NUMBER;
}

inline int openmp() {
    printf(">>> OpenMP\n");
    printf("1. DATA TYPE      : int\n");
    printf("2. VECTORS SIZE   : %d\n", NMAX);
    printf("3. VECTORS NUMBER : %d\n", 2);
    printf("4. THREADS NUMBER : [2, 4, 8]\n");
    printf("5. PARAMETER Q    : %d\n\n", Q);

    omp_set_num_threads(5);

    int* a = new int[NMAX];
    int* b = new int[NMAX];

    for (int i = 0; i < NMAX; ++i) {
        a[i] = 1;
        b[i] = 2;
    }

    double itime = init_time();
    double stime = sequental_time(a, b);
    double rtime = reduction_time(a, b);
    double atime = atomic_time   (a, b);
    double ctime = critical_time (a, b);

    printf("\n>>> TIME OF WORK\n");
    printf("SEQUENTAL : %f\n", stime);
    printf("REDUCTION : %f\n", rtime);
    printf("ATOMIC    : %f\n", atime);
    printf("CRITICAL  : %f\n\n", ctime);

    printf("\n>>> ACCELERATION W/I INIT\n");
    printf("REDUCTION : %f\n", stime / rtime);
    printf("ATOMIC    : %f\n", stime / atime);
    printf("CRITICAL  : %f\n\n", stime / ctime);

    printf("\n>>> ACCELERATION W/O INIT\n");
    printf("REDUCTION : %f\n", stime / (rtime - itime));
    printf("ATOMIC    : %f\n", stime / (atime - itime));
    printf("CRITICAL  : %f\n\n", stime / (ctime - itime));

    delete[] a;
    delete[] b;

    return 0;
}