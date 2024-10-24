cmake_minimum_required(VERSION 3.6)
project(ParallelProgramming)

set(SOURCE_FILES src/laboratory_work1.h
        src/laboratory_work2.h
        src/main.cpp
        src/lab2/openmp.h
        src/lab2/mpi-point-to-point.h
        src/lab2/utils.h
        src/lab2/mpi-collective.h
)
add_executable(ParallelProgramming ${SOURCE_FILES})

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
project(ParallelProgramming)

target_link_libraries(ParallelProgramming ${MPI_C_LIBRARIES})

message(STATUS "Checking OpenMP")
find_package(OpenMP)
IF(OPENMP_FOUND)
    message("Found OpenMP!")
    # add flags for OpenMP
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE()
    message("Missed OpenMP!")
ENDIF()

set(CMAKE_CXX_STANDARD 26)

