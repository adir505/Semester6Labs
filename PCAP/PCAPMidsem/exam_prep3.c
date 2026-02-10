//Program to scatter an array from root using buffered send and receive it's cube from worker processes.
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int buffer_size = (sizeof(int) + MPI_BSEND_OVERHEAD) * (size - 1);
    void* buffer = malloc(buffer_size);
    MPI_Buffer_attach(buffer, buffer_size);
    
    if(!rank){
        fprintf(stdout, "Rank 0: Processing cubes...\n");
        int send_array[size - 1], i, recv_array[size - 1];
        for(i = 0; i < size - 1; i++){
            send_array[i] = i + 1;
            MPI_Bsend(&send_array[i], 1, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
        }
        for(i = 0; i < size - 1; i++){
            MPI_Recv(&recv_array[i], 1, MPI_INT, i + 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fprintf(stdout, "Rank %d sent: %d\n", i + 1, recv_array[i]);
        }
    }
    else{
        int recv_val;
        MPI_Recv(&recv_val, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        recv_val *= recv_val * recv_val;
        MPI_Send(&recv_val, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Buffer_detach(&buffer, &buffer_size);
    MPI_Finalize();
    return 0;
}