#include <stdio.h>
#include <mpi.h>

void check_mpi_error(int err, char* msg){
    if(err != MPI_SUCCESS){
        int err_len;
        char err_string[MPI_MAX_ERROR_STRING];
        MPI_Error_string(err, err_string, &err_len);
        fprintf(stderr, "[MPI_ERROR]: %s: %s\n", msg, err_string);
    }
}

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank, size, err;
    err = MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
    check_mpi_error(err, "Setting error handler failed");
    err = MPI_Comm_size(MPI_COMM_WORLD, &size);
    check_mpi_error(err, "MPI_Comm_size failed");
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    check_mpi_error(err, "MPI_Comm_rank failed");

    int local = (rank + 1) * 2, global_sum, global_maximum;
    err = MPI_Reduce(&local, &global_sum, 1 , MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    check_mpi_error(err, "MPI_Reduce failed");
    err = MPI_Reduce(&local, &global_maximum, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    check_mpi_error(err, "MPI_Reduce failed");


    if(!rank) fprintf(stdout, "Rank 0 Context:\nThe sum of all computed values is: %d\nThe max of all computed values is : %d\n", global_sum, global_maximum);
    MPI_Finalize();
    return 0;
}