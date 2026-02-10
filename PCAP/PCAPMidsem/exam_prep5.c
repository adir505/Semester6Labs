#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank, size, workload, start_offset;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    workload = rand() % 10 + 1;
    MPI_Exscan(&workload, &start_offset, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    if(rank) fprintf(stdout, "Rank %d: Workload: %d Offset: %d\n", rank, workload, start_offset);
    else fprintf(stdout, "Rank 0: Workload: %d Offset: NA\n", workload);
    MPI_Finalize();
    return 0;
}