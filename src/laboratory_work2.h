//
// Created by mrtsstlrs on 11.10.2024.
//
#include <omp.h>
#include <iostream>
#include <vector>
#include <cstdlib>  // Для использования функции atoi

int laboratory_work2(int argc, char* argv[]) {
    // Проверка количества аргументов
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <num_threads> <vector_size> <num_vectors> <Q>" << std::endl;
        return 1;
    }

    // Чтение аргументов командной строки
    int num_threads = std::atoi(argv[1]);  // Количество потоков
    int N = std::atoi(argv[2]);  // Размерность векторов
    int K = std::atoi(argv[3]);  // Количество векторов
    int Q = std::atoi(argv[4]);  // Параметр Q

    // Инициализация векторов
    std::vector<std::vector<int>> vectors(K, std::vector<int>(N, 1));  // Заполним вектора значениями 1 для упрощения
    int total_sum = 0;
    double start_time, end_time;

    // Устанавливаем количество потоков
    omp_set_num_threads(num_threads);  // Количество потоков передается через консоль

    // --- Вариант с использованием reduction ---
    std::cout << "=== Reduction variant ===" << std::endl;
    start_time = omp_get_wtime();  // Начало измерения времени

    for (int q = 0; q < Q; ++q) {  // Повторяем расчет Q раз для увеличения нагрузки
        int sum = 0;
        #pragma omp parallel for reduction(+:sum)  // Опция reduction
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < N; ++j) {
                sum += vectors[i][j];
            }
        }
        total_sum = sum;  // Сохраняем результат
    }

    end_time = omp_get_wtime();  // Конец измерения времени
    std::cout << "Total Sum (Reduction): " << total_sum << std::endl;
    std::cout << "Time (Reduction): " << (end_time - start_time) << " seconds" << std::endl;

    // --- Вариант с использованием critical ---
    std::cout << "=== Critical variant ===" << std::endl;
    start_time = omp_get_wtime();  // Начало измерения времени

    for (int q = 0; q < Q; ++q) {  // Повторяем расчет Q раз для увеличения нагрузки
        int sum = 0;
        #pragma omp parallel for  // Параллелизация цикла
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < N; ++j) {
                #pragma omp critical  // Критическая секция
                {
                    sum += vectors[i][j];
                }
            }
        }
        total_sum = sum;  // Сохраняем результат
    }

    end_time = omp_get_wtime();  // Конец измерения времени
    std::cout << "Total Sum (Critical): " << total_sum << std::endl;
    std::cout << "Time (Critical): " << (end_time - start_time) << " seconds" << std::endl;

    // --- Вариант с использованием atomic ---
    std::cout << "=== Atomic variant ===" << std::endl;
    start_time = omp_get_wtime();  // Начало измерения времени

    for (int q = 0; q < Q; ++q) {  // Повторяем расчет Q раз для увеличения нагрузки
        int sum = 0;
        #pragma omp parallel for  // Параллелизация цикла
        for (int i = 0; i < K; ++i) {
            for (int j = 0; j < N; ++j) {
                #pragma omp atomic  // Используем atomic для сложения
                sum += vectors[i][j];
            }
        }
        total_sum = sum;  // Сохраняем результат
    }

    end_time = omp_get_wtime();  // Конец измерения времени
    std::cout << "Total Sum (Atomic): " << total_sum << std::endl;
    std::cout << "Time (Atomic): " << (end_time - start_time) << " seconds" << std::endl;

    return 0;
}

