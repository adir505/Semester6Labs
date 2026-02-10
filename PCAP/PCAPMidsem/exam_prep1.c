#include <stdio.h>
#include <mpi.h>
#define MAX_PROCESSES 10

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    int rank, size, n;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(size > MAX_PROCESSES){
        fprintf(stderr, "Maximum processes exceeded. Please try again.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    //Parallel logic. Root: Rank 0.
    if(!rank){
        int i;
        int final[size];
        fflush(stdout);
        fprintf(stdout, "Rank 0: Enter the number to be processed: ");
        scanf("%d", &n);
        for(i = 1; i < size; i++) MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        fprintf(stdout, "Rank 0: Processing...\nRank 0: Receiving final array... Finished.\n");
        for(i = 0; i < size - 1; i++){
            MPI_Recv(&final[i], 1, MPI_INT, i + 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fprintf(stdout, "Rank %d Processed: %d\n", i+1, final[i]);
        }
    }
    else{
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        n *= n;
        MPI_Send(&n, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}