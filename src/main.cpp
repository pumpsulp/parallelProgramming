#include "lab2/mpi-collective.h"
#include "lab2/mpi-point-to-point.h"
#include "lab2/openmp.h"

#include "mpi.h"  // для вызова MPI_Init и MPI_Finalize

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    openmp();

    mpi_collective();
    mpi_point_to_point();

    MPI_Finalize();

    return 0;
}
