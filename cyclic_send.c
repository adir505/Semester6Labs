//Program to read integer in process 0 which is sent to process 1,
//process 1 increments it and sends it to process 2, and so on.
//Finally, the last process sends it back to process 0 which prints it.
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int number;
    if (world_rank == 0) {
        // Initialize the number to send
        number = 0;
        printf("Process 0 sending number: %d\n", number);
        MPI_Ssend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        
        // Receive the final incremented number from the last process
        MPI_Recv(&number, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 received final number: %d\n", number);
    } else {
        // Receive the number from the previous process
        MPI_Recv(&number, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Increment the number
        number++;
        printf("Process %d incremented number to: %d\n", world_rank, number);
        
        // Send the incremented number to the next process or back to process 0
        int next_rank = (world_rank + 1) % world_size;
        MPI_Ssend(&number, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}