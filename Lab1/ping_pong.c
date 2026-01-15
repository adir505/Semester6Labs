#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // We need at least 2 processes for this to work
    if (world_size < 2) {
        if (world_rank == 0) printf("World size must be at least 2 for ping-pong!\n");
        MPI_Finalize();
        return 0;
    }

    int ping_pong_count = 0;
    int partner_rank = (world_rank + 1) % 2; // Rank 0 partner is 1, Rank 1 partner is 0

    if (world_rank == 0) {
        // Increment and send
        ping_pong_count++;
        MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
        printf("Rank 0 sent and incremented ping_pong_count %d to Rank 1\n", ping_pong_count);
    } else if (world_rank == 1) {
        // Receive, increment, and send back
        MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 1 received ping_pong_count %d from Rank 0\n", ping_pong_count);
        
        ping_pong_count++;
        MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
    }

    // Rank 0 receives the final "hit"
    if (world_rank == 0) {
        MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 0 received final ping_pong_count %d from Rank 1\n", ping_pong_count);
    }

    MPI_Finalize();
    return 0;
}