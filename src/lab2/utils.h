//
// Created by mrtsstlrs on 24.10.2024.
//

//  В качестве варианта выдается комбинация: тип, количество векторов,
//размерность вектора, [перечень количества процессов/потоков], параметр Q
// int,  3, 9 600 000, [2, 4, 8], 17

#include <ctime>
#define NMAX         600000
#define ITERS_NUMBER 20
#define Q            17


inline double sequental_time(int* a, int* b) {
    double sum = 0, total_time = 0;

    for (int cntr = 0; cntr < ITERS_NUMBER; ++cntr) {
        clock_t start_time = clock();

        for (int i = 0; i < NMAX; ++i) {
            for (int j = 0; j < Q; ++j) {
                sum = sum + a[i] + b[i];
            }
        }

        clock_t end_time = clock();
        total_time = total_time + (end_time - start_time);
    }

    sum = sum / (ITERS_NUMBER * Q);
    double time_of_work = total_time / CLOCKS_PER_SEC / ITERS_NUMBER;

    return time_of_work;
}


