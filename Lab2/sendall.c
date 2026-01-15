//Program to send message from process 0 to all other processes.
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    const int MAX_LEN = 100;
    char message[MAX_LEN];

    if (world_rank == 0) {
        // Initialize the message to send
        snprintf(message, MAX_LEN, "Hello from Rank 0");
        printf("Rank 0 sending message: %s\n", message);

        // Send the message to all other processes
        for (int i = 1; i < world_size; i++) {
            MPI_Send(message, MAX_LEN, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Receive the message from Rank 0
        MPI_Recv(message, MAX_LEN, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d received message: %s\n", world_rank, message);
    }

    MPI_Finalize();
    return 0;
}