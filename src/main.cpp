//
// Created by mrtsstlrs on 11.10.2024.
//

#include <omp.h>
// #include <iostream>
// #include <vector>
// #include <cstdlib>  // Для использования функции atoi

// int main(int argc, char* argv[]) {
//     // Проверка количества аргументов
//     if (argc != 5) {
//         std::cerr << "Usage: " << argv[0] << " <num_threads> <vector_size> <num_vectors> <Q>" << std::endl;
//         return 1;
//     }
//
//     // Чтение аргументов командной строки
//     int num_threads = std::atoi(argv[1]);  // Количество потоков
//     int N = std::atoi(argv[2]);  // Размерность векторов
//     int K = std::atoi(argv[3]);  // Количество векторов
//     int Q = std::atoi(argv[4]);  // Параметр Q
//
//     // Инициализация векторов
//     std::vector<std::vector<int>> vectors(K, std::vector<int>(N, 1));  // Заполним вектора значениями 1 для упрощения
//     int total_sum = 0;
//     double start_time, end_time;
//
//     // Устанавливаем количество потоков
//     omp_set_num_threads(num_threads);  // Количество потоков передается через консоль
//
//     // --- Вариант с использованием reduction ---
//     std::cout << "=== Reduction variant ===" << std::endl;
//     start_time = omp_get_wtime();  // Начало измерения времени
//
//     for (int q = 0; q < Q; ++q) {  // Повторяем расчет Q раз для увеличения нагрузки
//         int sum = 0;
//         #pragma omp parallel for reduction(+:sum)  // Опция reduction
//         for (int i = 0; i < K; ++i) {
//             for (int j = 0; j < N; ++j) {
//                 sum += vectors[i][j];
//             }
//         }
//         total_sum = sum;  // Сохраняем результат
//     }
//
//     end_time = omp_get_wtime();  // Конец измерения времени
//     std::cout << "Total Sum (Reduction): " << total_sum << std::endl;
//     std::cout << "Time (Reduction): " << (end_time - start_time) << " seconds" << std::endl;
//
//     // --- Вариант с использованием critical ---
//     std::cout << "=== Critical variant ===" << std::endl;
//     start_time = omp_get_wtime();  // Начало измерения времени
//
//     for (int q = 0; q < Q; ++q) {  // Повторяем расчет Q раз для увеличения нагрузки
//         int sum = 0;
//         #pragma omp parallel for  // Параллелизация цикла
//         for (int i = 0; i < K; ++i) {
//             for (int j = 0; j < N; ++j) {
//                 #pragma omp critical  // Критическая секция
//                 {
//                     sum += vectors[i][j];
//                 }
//             }
//         }
//         total_sum = sum;  // Сохраняем результат
//     }
//
//     end_time = omp_get_wtime();  // Конец измерения времени
//     std::cout << "Total Sum (Critical): " << total_sum << std::endl;
//     std::cout << "Time (Critical): " << (end_time - start_time) << " seconds" << std::endl;
//
//     // --- Вариант с использованием atomic ---
//     std::cout << "=== Atomic variant ===" << std::endl;
//     start_time = omp_get_wtime();  // Начало измерения времени
//
//     for (int q = 0; q < Q; ++q) {  // Повторяем расчет Q раз для увеличения нагрузки
//         int sum = 0;
//         #pragma omp parallel for  // Параллелизация цикла
//         for (int i = 0; i < K; ++i) {
//             for (int j = 0; j < N; ++j) {
//                 #pragma omp atomic  // Используем atomic для сложения
//                 sum += vectors[i][j];
//             }
//         }
//         total_sum = sum;  // Сохраняем результат
//     }
//
//     end_time = omp_get_wtime();  // Конец измерения времени
//     std::cout << "Total Sum (Atomic): " << total_sum << std::endl;
//     std::cout << "Time (Atomic): " << (end_time - start_time) << " seconds" << std::endl;
//
//     return 0;
// }


#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (argc != 5) {
        if (ProcRank == 0) {
            std::cerr << "Usage: " << argv[0] << " <vector_size> <num_vectors> <Q> <num_processes>" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    int N = std::atoi(argv[1]);
    int K = std::atoi(argv[2]);
    int Q = std::atoi(argv[3]);
    int num_processes = std::atoi(argv[4]);

    std::vector<std::vector<int>> vectors(K, std::vector<int>(N, 1));

    double start_time, end_time;

    // --- Широковещательная рассылка данных (broadcast) ---
    if (ProcRank == 0) {
        std::cout << "=== Starting broadcast ===" << std::endl;
    }

    start_time = MPI_Wtime();

    for (int i = 0; i < K; ++i) {
        MPI_Bcast(vectors[i].data(), N, MPI_INT, 0, MPI_COMM_WORLD);
    }

    end_time = MPI_Wtime();
    if (ProcRank == 0) {
        std::cout << "Broadcast time: " << (end_time - start_time) << " seconds" << std::endl;
    }

    // "Точка-точка" (Point-to-Point)
    if (ProcRank == 0) {
        std::cout << "=== Point-to-Point ===" << std::endl;
    }

    start_time = MPI_Wtime();

    int k = N / ProcNum;
    int i1 = k * ProcRank;
    int i2 = k * (ProcRank + 1);

    if (ProcRank == ProcNum - 1) i2 = N;

    for (int q = 0; q < Q; ++q) {
        int ProcSum = 0;

        for (int i = 0; i < K; ++i) {
            for (int j = i1; j < i2; ++j) {
                ProcSum += vectors[i][j];
            }
        }

        int TotalSum;
        if (ProcRank == 0) {
            TotalSum = ProcSum;
            for (int i = 1; i < ProcNum; ++i) {
                int temp;
                MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                TotalSum += temp;
            }
            std::cout << "Total Sum (Point-to-Point): " << TotalSum << std::endl;
        } else {
            MPI_Send(&ProcSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    end_time = MPI_Wtime();
    if (ProcRank == 0) {
        std::cout << "Point-to-Point time: " << (end_time - start_time) << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}

