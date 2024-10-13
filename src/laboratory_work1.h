#include <math.h>
#include <omp.h>
#include "mpi.h"
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <stdio.h>

//MPI

int lr1(int argc, char* argv[])
{
    int rank, ranksize, i;
    MPI_Init(&argc, &argv);//
    //Определяем свой номер в группе:
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //
    //Определяем размер группы:
    MPI_Comm_size(MPI_COMM_WORLD, &ranksize);//
    printf("Hello world from process %d from total number of %d\n", rank, ranksize);
    MPI_Finalize(); //
    return 0;
}



//OpenMP

// int main(int argc, char* argv[])
// {
//     omp_set_num_threads(15);
//     int nTheads, theadNum;
// #pragma omp parallel  private(nTheads, theadNum)
//     {
//         nTheads = omp_get_num_threads();
//         theadNum = omp_get_thread_num();
//         printf("OpenMP thread %d from %d threads \n", theadNum, nTheads);
//     }
//     return 0;
// }
